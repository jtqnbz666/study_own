require "test1"

 require "hignmethod"

c1 = test1.new(0, 1)
c2 = test1.new(1, 2)

-- ret =test1.add(c1, c2)
ret = hignmethod.add(c1, c2)    
hignmethod.print(ret.r, ret.i)
print(ret.r, ret.i)
