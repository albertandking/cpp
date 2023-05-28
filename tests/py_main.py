from lib import example
from lib import add
from lib import options
from time import perf_counter

start_time = perf_counter()

result = example.add()
print(result)

result = add.add(100, 200)
print(result)

result = options.cal_option()
print(result)

end_time = perf_counter()
print(end_time - start_time)
