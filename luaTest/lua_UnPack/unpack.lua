do 
    arrayData = {a = "test", "b", "c", "d"}  --对于unpack，第一个会被忽略
    
    function returnMoreValues()
        return 1,2,3
    end

    a, b, c = returnMoreValues()
    
    print(a, b, c)
    print(returnMoreValues())

    print(arrayData) ; --打印的是一个地址
     print(unpack(arrayData)) --打印处所有元素
    print(unpack(arrayData, 2)) --从第二个元素开始打印
    print(arrayData["a"])
  end
