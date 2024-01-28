## go

8.Content-Type区别

主要关心两种，application/x-www-form-urlencoded(就是说的表单)和application/json，如果是后者的话，直接用json.Unmarshal把body转为需要的类型(一般是map[string]interface{}或自定义类型)

7.url.ParseQuery

前提是**application/x-www-form-urlencoded**类型
特别注意使用了这个函数之后，本来是整数的，但都被转为了string，所以文档里面如果写的是整数，但通过这种方式最终得到的map[string]interface{}全是string类型了(因为url.ParseQuery会转为string), 以下目标1不受影响，目标2就有这个问题

~~~go
目的1:将收到的数据转为map[string]interface{}对象
一种是bilibili的，格式为url?data="{json串}"
1.param, err := url.ParseQuery(string(body))
2.dataJsonStr := param.Get("data")
3.var params map[string]interface{}
json.Unmarshal([]byte(dataJsonStr), &params)

另一种是tiktok,格式为url?A=666&sign=EXC%3d
其实这个%3d就是=号，但使用了url编码，因为=算特殊字符
1.param, _ := url.ParseQuery(string(body))
2.result := make(map[string]interface{})
	for key, values := range param {
		if len(values) > 0 {
			result[key] = values[0]
		}
	}

注意：上边说的string(body)对应的是url?后边的那些数据
~~~

case2:将收到的数据转为自定义的结构
~~~go
params, err := url.ParseQuery(body)
jsonData, err := json.Marshal(params)
var request NotificationRequest //自定义结构
json.Unmarshal(jsonData, &request); err != 
~~~



6.curl操作

~~~
get方法：curl -X GET <URL>
post方法：curl -X POST -d "key1=value1&key2=value2" <URL>
请求头：curl -H "Content-Type: application/json" <URL>
下载文件：curl -O <file_url>
curl -fsSL是一个常用的curl命令的组合。解析这个命令的含义如下：
-f选项，或--fail，表示当请求失败时不显示错误信息，而是返回非零的退出码。
-s选项，或--silent，表示在命令执行过程中不显示进度和错误信息，只显示结果。
-S选项，或--show-error，表示在发生错误时显示错误信息。
-L选项，或--location，表示在遇到HTTP重定向时自动跟随重定向。
~~~

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
} // 这里注意，如果用map[string]interface{}接收，所有数字类型都默认是floate64
~~~

3.如果要给post方法设置head，需要创建一个http对象

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

5.https://line3-game-api-adapter-na-sh.biligame.net/api/server/session.verify?game_id=10456&merchant_id=3693&server_id=9624&uid=172372941&version=1&access_key=117f7c2c83eb7a116d701ee0dc75f3c1&sign=e502ca94bd49b4aa7cdaf1f076abee6b×tamp=1701865189881

比如这个url，如果都是post方法的情况下， 直接这样写跟把url写成https://line3-game-api-adapter-na-sh.biligame.net/api/server/session.verify，然后其他参数写到body的x-www-form-urlencoded一样的效果，**不管你怎么写url，只要是post方法发送的时候都会把参数放到body中传输，而get是直接放到url上传输的**

比如现在接收方有个结构体是

~~~go
type FcRequest struct {
	Header    map[string]string `json:"headers"`
	Body      string            `json:"body"`
	Param     map[string]string `json:"queryParameters"`
	Path      string            `json:"rawPath"`
	PathParam []string
}
那么post方法的数据会被解析道Body对象中， 而如果是使用的get方法，就会在Param中
~~~

