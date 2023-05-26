#include <pybind11/pybind11.h>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <iostream>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <boost/date_time.hpp>
#include <ql/qldefines.hpp>
#if !defined(BOOST_ALL_NO_LIB) && defined(BOOST_MSVC)
#  include <ql/auto_link.hpp>
#endif
#include <ql/instruments/vanillaswap.hpp>
#include <ql/instruments/makevanillaswap.hpp>
#include <ql/pricingengines/swap/discountingswapengine.hpp>
#include <ql/pricingengines/swap/cvaswapengine.hpp>
#include <ql/termstructures/yield/piecewiseyieldcurve.hpp>
#include <ql/termstructures/yield/ratehelpers.hpp>
#include <ql/termstructures/credit/interpolatedhazardratecurve.hpp>
#include <ql/indexes/ibor/euribor.hpp>
#include <ql/time/calendars/target.hpp>
#include <ql/time/daycounters/actualactual.hpp>
#include <ql/time/daycounters/actual360.hpp>

#include <iomanip>

using namespace std;
using namespace QuantLib;
using namespace rapidjson;

int add() {
    boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
    std::cout << "Current time: " << boost::gregorian::to_iso_extended_string(now.date()) << std::endl;

    boost::gregorian::date today = boost::gregorian::day_clock::local_day();
    std::cout << "Today's date: " << today << std::endl;

    boost::gregorian::date nextWeek = today + boost::gregorian::weeks(1);
    std::cout << "Date one week from now: " << nextWeek << std::endl;


    std::string input = "Hello,World,Boost";
    std::vector<std::string> tokens;

    boost::split(tokens, input, boost::is_any_of(","));


    for (const auto& token : tokens) {
        std::cout << token << std::endl;
    }

    const char *json = "{\"project\":\"rapidjson\",\"stars\":100}";
    Document d;
    d.Parse(json);

    // 2. Modify it by DOM.
    Value &s = d["stars"];
    s.SetInt(s.GetInt() + 1);

    // 3. Stringify the DOM
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    d.Accept(writer);

    // Output {"project":"rapidjson","stars":11}
    std::cout << buffer.GetString() << std::endl;

    vector<int> v;
    v.push_back(10);
    v.push_back(20);
    v.push_back(30);
    v.push_back(40);

    for (int ii = 0; ii < 10; ii++) {
        v.push_back(ii);
    }

    for (int jj: v) {
        cout << jj << endl;
    }

    try {

        std::cout << std::endl;

        Calendar calendar = TARGET();
        Date todaysDate(10, March, 2004);
        // must be a business day
        todaysDate = calendar.adjust(todaysDate);

        Settings::instance().evaluationDate() = todaysDate;

        ext::shared_ptr<IborIndex>  yieldIndx(new Euribor3M());
        Size tenorsSwapMkt[] = {5, 10, 15, 20, 25, 30};

        // rates ignoring counterparty risk:
        Rate ratesSwapmkt[] = {.03249, .04074, .04463, .04675, .04775, .04811};

        vector<ext::shared_ptr<RateHelper> > swapHelpers;
        for(Size i=0; i<sizeof(tenorsSwapMkt)/sizeof(Size); i++)
            swapHelpers.push_back(ext::make_shared<SwapRateHelper>(
                    Handle<Quote>(ext::shared_ptr<Quote>(
                            new SimpleQuote(ratesSwapmkt[i]))),
                    tenorsSwapMkt[i] * Years,
                    TARGET(),
                    Quarterly,
                    ModifiedFollowing,
                    ActualActual(ActualActual::ISDA),
                    yieldIndx));

        ext::shared_ptr<YieldTermStructure> swapTS(
                new PiecewiseYieldCurve<Discount,LogLinear>(
                        2, TARGET(), swapHelpers, ActualActual(ActualActual::ISDA)));
        swapTS->enableExtrapolation();

        ext::shared_ptr<PricingEngine> riskFreeEngine(
                ext::make_shared<DiscountingSwapEngine>(
                        Handle<YieldTermStructure>(swapTS)));

        std::vector<Handle<DefaultProbabilityTermStructure> >
                defaultIntensityTS;

        Size defaultTenors[] = {0, 12, 36, 60, 84, 120, 180, 240, 300,
                                360};// months
        // Three risk levels:
        Real intensitiesLow[] = {0.0036, 0.0036, 0.0065, 0.0099, 0.0111,
                                 0.0177, 0.0177, 0.0177, 0.0177, 0.0177,
                                 0.0177};
        Real intensitiesMedium[] = {0.0202, 0.0202, 0.0231, 0.0266, 0.0278,
                                    0.0349, 0.0349, 0.0349, 0.0349, 0.0349,
                                    0.0349};
        Real intensitiesHigh[] = {0.0534, 0.0534, 0.0564, 0.06, 0.0614, 0.0696,
                                  0.0696, 0.0696, 0.0696, 0.0696, 0.0696};
        // Recovery rates:
        Real ctptyRRLow = 0.4, ctptyRRMedium = 0.35, ctptyRRHigh = 0.3;

        std::vector<Date> defaultTSDates;
        std::vector<Real> intesitiesVLow, intesitiesVMedium, intesitiesVHigh;

        for(Size i=0; i<sizeof(defaultTenors)/sizeof(Size); i++) {
            defaultTSDates.push_back(TARGET().advance(todaysDate,
                                                      Period(defaultTenors[i], Months)));
            intesitiesVLow.push_back(intensitiesLow[i]);
            intesitiesVMedium.push_back(intensitiesMedium[i]);
            intesitiesVHigh.push_back(intensitiesHigh[i]);
        }

        defaultIntensityTS.emplace_back(ext::shared_ptr<DefaultProbabilityTermStructure>(
                new InterpolatedHazardRateCurve<BackwardFlat>(defaultTSDates, intesitiesVLow,
                                                              Actual360(), TARGET())));
        defaultIntensityTS.emplace_back(ext::shared_ptr<DefaultProbabilityTermStructure>(
                new InterpolatedHazardRateCurve<BackwardFlat>(defaultTSDates, intesitiesVMedium,
                                                              Actual360(), TARGET())));
        defaultIntensityTS.emplace_back(ext::shared_ptr<DefaultProbabilityTermStructure>(
                new InterpolatedHazardRateCurve<BackwardFlat>(defaultTSDates, intesitiesVHigh,
                                                              Actual360(), TARGET())));

        Volatility blackVol = 0.15;
        ext::shared_ptr<PricingEngine> ctptySwapCvaLow =
                ext::make_shared<CounterpartyAdjSwapEngine>(
                        Handle<YieldTermStructure>(swapTS),
                        blackVol,
                        defaultIntensityTS[0],
                        ctptyRRLow
                );

        ext::shared_ptr<PricingEngine> ctptySwapCvaMedium =
                ext::make_shared<CounterpartyAdjSwapEngine>(
                        Handle<YieldTermStructure>(swapTS),
                        blackVol,
                        defaultIntensityTS[1],
                        ctptyRRMedium);
        ext::shared_ptr<PricingEngine> ctptySwapCvaHigh =
                ext::make_shared<CounterpartyAdjSwapEngine>(
                        Handle<YieldTermStructure>(swapTS),
                        blackVol,
                        defaultIntensityTS[2],
                        ctptyRRHigh);

        defaultIntensityTS[0]->enableExtrapolation();
        defaultIntensityTS[1]->enableExtrapolation();
        defaultIntensityTS[2]->enableExtrapolation();



        // fixed leg
        Frequency fixedLegFrequency = Quarterly;
        BusinessDayConvention fixedLegConvention = ModifiedFollowing;
        DayCounter fixedLegDayCounter = ActualActual(ActualActual::ISDA);
        DayCounter floatingLegDayCounter = ActualActual(ActualActual::ISDA);

        Swap::Type swapType = Swap::Payer;
        ext::shared_ptr<IborIndex> yieldIndxS(
                new Euribor3M(Handle<YieldTermStructure>(swapTS)));
        std::vector<VanillaSwap> riskySwaps;
        for(Size i=0; i<sizeof(tenorsSwapMkt)/sizeof(Size); i++)
            riskySwaps.push_back(MakeVanillaSwap(tenorsSwapMkt[i]*Years,
                                                 yieldIndxS,
                                                 ratesSwapmkt[i],
                                                 0*Days)
                                         .withSettlementDays(2)
                                         .withFixedLegDayCount(fixedLegDayCounter)
                                         .withFixedLegTenor(Period(fixedLegFrequency))
                                         .withFixedLegConvention(fixedLegConvention)
                                         .withFixedLegTerminationDateConvention(fixedLegConvention)
                                         .withFixedLegCalendar(calendar)
                                         .withFloatingLegCalendar(calendar)
                                         .withNominal(100.)
                                         .withType(swapType));

        cout << "-- Correction in the contract fix rate in bp --" << endl;
        /* The paper plots correction to be substracted, here is printed
           with its sign
        */
        for(Size i=0; i<riskySwaps.size(); i++) {
            cout << fixed << setprecision(3);
            cout << setw(4);
            riskySwaps[i].setPricingEngine(riskFreeEngine);
            // should recover the input here:
            Real nonRiskyFair = riskySwaps[i].fairRate();
            cout << tenorsSwapMkt[i];
            cout << setw(5);

            cout << " | " << io::rate(nonRiskyFair);
            cout << fixed << setprecision(2);
            cout << setw(5);
            // Low Risk:
            riskySwaps[i].setPricingEngine(ctptySwapCvaLow);
            cout << " | " << setw(6)
                 << 10000.*(riskySwaps[i].fairRate() - nonRiskyFair);
            //cout << " | " << setw(6) << riskySwaps[i].NPV() ;

            // Medium Risk:
            riskySwaps[i].setPricingEngine(ctptySwapCvaMedium);
            cout << " | " << setw(6)
                 << 10000.*(riskySwaps[i].fairRate() - nonRiskyFair);
            //cout << " | " << setw(6) << riskySwaps[i].NPV() ;

            riskySwaps[i].setPricingEngine(ctptySwapCvaHigh);
            cout << " | " << setw(6)
                 << 10000.*(riskySwaps[i].fairRate() - nonRiskyFair);
            //cout << " | " << setw(6) << riskySwaps[i].NPV() ;

            cout << endl;
        }

        cout << endl;

    } catch (exception& e) {
        cerr << e.what() << endl;
        return 1;
    } catch (...) {
        cerr << "unknown error" << endl;
        return 1;
    }
    return 0;
}

PYBIND11_MODULE(example, m) {
    m.doc() = "pybind11 example plugin"; // optional module docstring

    m.def("add", &add, "A function that adds two numbers");
}