

http小知识

~~~shell
4.get用于读，post一般是涉及到修改服务器资源或者上报，跟有无请求参数无关。
3.URL(统一资源定位符)是URI(统一资源标识符)的子集，后者更广泛。
1.head中的Host字段记录了请求的域名信息，如sandbox-shengji.duole.com，会带端口号，除非默认用80端口则只有域名，总之就是输入的url中//之后，路径之前的那一截
~~~

http遇到的问题

~~~shell
2.企业微信扫码登录的时候，safari浏览器直接看到了负载均衡(监听的443,https类型)后边nginx的端口，当时nginx监听的80端口，导致safari生成的回调地址变为了http类型导致流量根本无法到达nginx服务，后边把nginx监听的端口改为了443解决的问题，遇到此类问题，用不同的浏览器打开扫码登录界面，对比不同浏览器的url的回调地址是否有区别，比如有的是http，有的是https。
1.如果url访问失败，可能是两个问题，url编码方式有问题，或者url被截断了，把域名换成自己的服务器，请求过来看看发送的数据具体是什么，比如;被编码为%3B，有些平台会认为%是特殊字符会把%编码为%25，也就变成了%253B，解决方法是：提供给前端的就是未进行编码的初始格式。
~~~



url传输二进制数据

~~~shell
两种方式：
1.用base64处理二进制为字符 
2.直接对二进制数据进行url编码
from  urllib.parse import quote, unquote, urlparse, parse_qs
binary_data = b'\x89\x50\x4E\x47'
encoded_data = quote(binary_data.decode('latin1'))
url = f'https://example.com/data?binary={encoded_data}'
print(url) # https://example.com/data?binary=%89PNG
# 解析URL取出二进制数据
parsed_url = urlparse(url)
query_params = parse_qs(parsed_url.query)
# 提取并解码数据
encoded_data = query_params['binary'][0]
decoded_data = unquote(encoded_data).encode('latin1')
print(decoded_data)  # 输出原始二进制数据
# 注意以上操作都是对url实际数据的操作，并且没有涉及到url传输的过程，https://example.com/data?binary=%89PNG在发送出去的时候会再次进行整体的url编码，刚才的操作只是把二进制数据放到url中。

# 发送这个url
url = 'https://example.com/data?binary=%89PNG' # 后边是处理后的二进制数据
print(quote(url5, safe=':/?&=')) # 结果为：https://example.com/data?binary=%2589PNG   这是部分编码的结果。
~~~

url编码

~~~shell
7.url这部分都会进行url编码，跟方法没关系，如果是post方法，把body指定格式为x-www-form-urlencoded，那么body部分也会进行url编码(如果是application/json，那就是json格式)，如果用的是参数形式，那么post方法需要的参数会全部拼到url中，此时类似于get的形式了，并且body里面没有任何数据，所以有时候post方法不一定是从body中取数据，可能都拼在了url中。
6.对于汉字内容，不像&必须在业务层进行url编码，浏览器进行url统一编码的时候会对她进行url编码，对方收到后解析出来的就还是汉字内容，如果在业务层就进行了一次url编码，对方收到之后需要进行一次url解码才能拿到汉字内容。
5.参数中?p、?p=、?p=""是等价的，都表示空值，但有字段。
4.编码规则：比如&的ASCII码为38，对应的16进制为&26，所以被编码为%26，汉字如"设"的 UTF-8 编码是 E8 AD BE，会被编码为"%E8%AD%BE"。
3.url只能包含ASCII字符，但如&在url有独特了含义，还有空格等，仍需要编码
2.如果参数的值包含&(或者其他在url有有特殊含义的字符)，必须在业务层就对它进行url编码，因为&在url中有特殊含义表示分割参数，比如在postman中输入参数值为v1&v2时，url会自动转为v1%26v2，当放到url中依然还是v1&v2时，会被认为是v1和v2两个参数，只是没有值。
1.完全编码和非完全编码解析结果都是一样的。
url1='https://test?p_userid=123&p_&p_%E8%AE%BE%E5%A4%87%E5%8F%B7=EF27'
url2='https%3A%2F%2Ftest%3Fp_userid%3D123%26p_%26p_%E8%AE%BE%E5%A4%87%E5%8F%B7%3DEF27'
print(unquote(url3)) # https://test?p_userid=123&p_&p_设备号=EF27
print(unquote(url4)) # https://test?p_userid=123&p_&p_设备号=EF27
url1在编码时忽略了对':/?&='的编码，算部分编码，比如python中的quote(url, safe=':/?&=')，这样保留了一定的可读性，但可以观察到这个url中有一个多余的&p_，它其实也被作为一个参数了，只是没有对应的值，用postman就可以试出来，只输入key。
~~~

http返回码

~~~
304http缓存设计， 如果客户端缓存时间到期了，去服务器对比后发现，服务器还是原来客户端缓存的数据，这时候服务器就回应304，不带数据，告诉客户端继续使用以前缓存的那份数据
200， 201(表示服务器已经成功处理，并且创建了新的资源，一般出现在post请求的返回上)， 301(永久重定向)， 302(临时重定向)， 401(未认证用户信息)， 403(权限不够) ，500(服务器内部未知错误，比如web.py配置了某路由，但对应的类名写错了，如果直接不写路由，会返回404)，501（不支持的请求方法）502(比如nginx作为反向代理时(或clb)，接受到业务服务器的不正确返回时，通常是业务服务器挂了)，503(负载大，维护，服务器自己的问题，可以理解为clb自己出问题了)， 504(超过了指定的时间,比如阿里云http负载均衡器等待后端服务返回超时(http的clb等价于一个nginx，所以nginx后边嵌套一个nginx也是一样的场景)，1.一般是指clb后边的服务器响应慢而不是clb本身的问题, 2.clb不限制，但后边的服务器安全组未放通，没挂vpn就访问数数平台，clb不拦，但后边的数数服务器拦住了，简单说就是访问不到后端服务器，不存在或者被限制了), 505(不支持的http协议，比如过时的版本)
~~~

快速搭建一个http服务器接收请求，查看头字段

~~~js
apt install npm
编写test.js文件
const http = require('http');
const server = http.createServer((req, res) => {
  console.log(req.headers);
  // 设置 Cookie
  res.setHeader('Set-Cookie', 'shengji_session=e9d9fc1d2bcf7893901c15ac8979fb1765ea4e11; HttpOnly; Path=/');
  res.end('Hello World');
});
server.listen(8881, () => {
  console.log('Server is listening on port 8881');
});
// 执行 node test.js运行服务
~~~

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

~~~shell
# url要用引号包起来
# -X 是 --request的缩写
get方法：curl <URL>  
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

