## go

1.map[string]interface{} 概括了一个通用json串， 如果接收到一个这样的对象，就使用json.Marshal先搞成json对象，再用json.Unmarshal解析为你需要的对象
例如：

~~~go
query 是 map[string]interface{}类型
queryContent, _ := json.Marshal(query)
request := message.FcRequest{}
_ = json.Unmarshal(queryContent, &request)
~~~

2.io.ReadAll()读出来的是[]byte类型可以直接用，如果知道里面的大致内容可以直接用json.Unmarshal进行解析
例如
MiOrder是我自己写的结构体，因为我知道我会接收到什么内容，比如resp.Body内容为：{"uid":"2023112800235103","appId":2882303761520282579,"cpOrderId":"20231128220517857725190"}

~~~go
body, err := ioutil.ReadAll(resp.Body)
var order MiOrder
err = json.Unmarshal(body, &order)

如果不写MiOrder，可以用一个万能的方式来接收结果
比如:
var result map[string]interface{}
json.NewDecoder(resp.Body).Decode(&result)

if result["errcode"].(float64) == CodeSuccess {
  return true
}
~~~

3.如果要给post方法设置head，需要创建一个hppt对象

~~~go
client := &http.Client{}
data := strings.NewReader(params1.Encode())
req, err := http.NewRequest("POST", BiliVerifyUrl, data)
// 设置header
req.Header.Set("Content-Type", "application/x-www-form-urlencoded")
req.Header.Set("User-Agent", "Mozilla/5.0 GameServer")
resp, err := client.Do(req)
~~~

4.给post方法设置入参， 这样能保证需要是数的时候就是数而不是字符串

~~~go
params := map[string]interface{}{
		"access_key":  AccessKey,
		"uid":         Bid,
		"game_id":     1,
		"merchant_id": 1,
		"version":     1,
		"timestamp":   timeUtil.MakeNowTimeStamp(),
	}
	// 用 post
	// 构建请求参数
	postParams := url.Values{}
	for key, val := range params {
		postParams.Add(key, fmt.Sprintf("%v", val))
	}
	postParams.Set("sign", getBiliSign(params))
~~~



