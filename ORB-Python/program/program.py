import gc
a = 0.5
while a < 10000:
    a = a + 0.5
    gc.collect()
    print(gc.mem_free())