package main

import (
	"encoding/base64"
	"encoding/json"
	"fmt"
	"io/ioutil"
	"net/http"
	"net/url"
	"os"
	"strconv"
	"time"
)

func sendTuchuang(turePath string) string {
	fmt.Println("开始上传图床:" + turePath)
	//fmt.Println(turePath, origin, strings.Index(turePath, origin))
	//if strings.Index(turePath, origin) > -1 {
	var streams []byte
	//if strings.Contains(turePath, "http") {
	//	var images []string
	//	images = append(images, turePath)
	//	streams, _ = json.Marshal(images)
	//} else {
		//上传图床
		fileC, err := os.Open(turePath)
		if err != nil {
			panic(err)
		}
		defer fileC.Close()
		content, err := ioutil.ReadAll(fileC)

		var images []string
		images = append(images, base64.RawStdEncoding.EncodeToString(content))
		streams, _ = json.Marshal(images)
	//}

	//fmt.Println(string(streams))

	resp, err := http.PostForm("http://api.v3.picasso.qhimg.com/v3/app/tuchuang_wenku/upload_pics/",
		url.Values{
			"APP":       {"tuchuang_wenku"},
			"VER":       {"4.0.0.0"},
			"RULES":     {"{\"demo\":[]}"},
			"IMGSTREAM": {string(streams)},
			"SIGN":      {md5sum(strconv.FormatInt(time.Now().Unix(), 10))},
			"TYPE":      {"0"},
		},
	)

	if err != nil {
		// handle error
		fmt.Println(err)
		return ""
	}

	defer resp.Body.Close()
	body, err := ioutil.ReadAll(resp.Body)
	if err != nil {
		// handle error
		fmt.Println("上传图床失败:" + turePath)
		fmt.Println(err)
		return ""
	}
	//fmt.Println(string(body))

	//fmt.Println("rm " + turePath)
	//fmt.Println(os.Remove(turePath))

	return string(body)
	//}
	//return ""
}
