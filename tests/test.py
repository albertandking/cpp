from packages import example
from packages import add
from packages import options
from time import perf_counter

start_time = perf_counter()


result = example.add()
print(result)

result = add.add(100, 200)
print(result)

result = options.cal_option()
print(result)

end_time = perf_counter()
print(end_time-start_time)