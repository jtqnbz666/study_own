是http的一种升级协议，可以理解为封装了epoll(linux平台下)，支持双向互发消息，并且是长连接的，http是请求响应式的，只能单向请求。

js代码案例（npm install ws下载websocket模块）

client(html文件直接在浏览器打开)

~~~js
// 可以进入开发者模式，选择console看到消息，如果要发送，因为代码中用的名字叫socket，直接输入socket.send('hhha');即可发送到服务器。
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>WebSocket Example</title>
</head>
<body>
    <script>
        // 创建 WebSocket 连接
        const socket = new WebSocket('ws://8.130.49.219:8080');

        // 连接打开时执行
        socket.onopen = function(event) {
            console.log('WebSocket connection opened');
            socket.send('Hello Server!');
        };

        // 接收到消息时执行
        socket.onmessage = function(event) {
            console.log('Message from server ', event.data);
        };

        // 连接关闭时执行
        socket.onclose = function(event) {
            console.log('WebSocket connection closed');
        };

        // 发生错误时执行
        socket.onerror = function(error) {
            console.error('WebSocket error: ', error);
        };
    </script>
</body>
</html>
~~~

server（node test.js启动）

~~~js
const WebSocket = require('ws');
const readline = require('readline');
const server = new WebSocket.Server({ port: 8080 });
server.on('connection', ws => {
    console.log('Client connected');

    ws.on('message', message => {
        const textMessage = message.toString();
    		console.log('Received:', textMessage);
    });

    ws.on('close', () => {
        console.log('Client disconnected');
    });
});

// 设置终端输入
const rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout
});

// 监听终端输入并发送给所有连接的客户端
rl.on('line', (input) => {
    server.clients.forEach(client => {
        if (client.readyState === WebSocket.OPEN) {
            client.send(input);
        }
    });
});

~~~

