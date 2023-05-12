package main

import (
	"bufio"
	"flag"
	"fmt"
	"io"
	"math/rand"
	"os"
	"strconv"
	"strings"
	"time"

	"gitee.com/fengling/gotools/simplejson"
)

var redo = flag.Int("redo", 0, "0 不重新转换，1 重新转换")
var option = flag.String("option", "xqppt", "site站点")
var pageNumber = flag.Int("pageNumber", 0, "页码数")
var docloadUrl = flag.String("docloadUrl", "", "docloadUrl")
var loc = flag.String("loc", "", "loc")
var docId = flag.String("docId", "", "docId")

var docDownloadUrl = flag.String("docDownloadUrl", "https://www.xqppt.com/api_suda/wenku/index?op={}&url={}&id={}&time={}&sign={}", "docDownloadUrl")

//var docDownloadUrl = flag.String("docDownloadUrl", "http://api.lizippt.com/360/getFile?op={}&url={}&id={}&time={}&sign={}", "docDownloadUrl")

var imgDownloadUrl = flag.String("imgDownloadUrl", "https://www.xqppt.com/api_suda/wenku/createimg?url={}&doc_id={}&k={}", "imgDownloadUrl")

//var imgDownloadUrl = flag.String("imgDownloadUrl", "http://api.lizippt.com/360/getImg?url={}&id={}&k={}&time={}", "imgDownloadUrl")

var docFormat = flag.String("docFormat", "", "docFormat")

func main() {
	flag.Parse()

	if (*pageNumber) > 0 && len(*docDownloadUrl) > 0 && len(*imgDownloadUrl) > 0 && len(*docId) > 0 && len(*loc) > 0 && len(*docFormat) > 0 {
		fmt.Println("111111111111")
		//running(1, *pageNumber, *docDownloadUrl, *imgDownloadUrl, *docId, *loc, *docFormat)
	} else {
		if *option == "tgppt" {
			*docDownloadUrl = "http://api.lizippt.com/360/getFile?op={}&url={}&id={}&time={}&sign={}"
			*imgDownloadUrl = "http://api.lizippt.com/360/getImg?url={}&id={}&k={}&time={}"
		}

		var inputReader *bufio.Reader
		var input string
		var err error
		//mapred_tip_id := os.Getenv("mapred_tip_id")
		//mapred_tip_ids := strings.Split(mapred_tip_id, "_r_")
		//curr_mapred_tip_id, err := strconv.Atoi(mapred_tip_ids[1])
		if err != nil {
			return
		}
		rand.Seed(time.Now().UnixNano())
		inputReader = bufio.NewReader(os.Stdin)
		for {
			//fmt.Println("mapred_tip_id:" + fmt.Sprintf("%v\n", curr_mapred_tip_id))

			input, err = inputReader.ReadString('\n')

			if err == io.EOF {
				break
			}

			if len(input) < 30 {
				continue
			}
			//1	{"data":{"url":{"data":{"display":{"id":"1","format":"2","pageNumber":"3","docImage":"4"}}}}}
			in := strings.Split(input, "\t")
			js, _ := simplejson.NewJson([]byte(in[1]))
			js_loc := ""

			//9594
			*docId, _ = js.Get("data").Get("url").Get("data").Get("display").Get("id").String()
			if len(*docId) <= 0 {
				fmt.Println("docId")
				continue
			}

			//xlsx 文件格式
			*docFormat, _ = js.Get("data").Get("url").Get("data").Get("display").Get("format").String()
			if len(*docFormat) <= 0 {
				fmt.Println("docFormat")
				continue
			}

			//"https:\/\/www.xqppt.com\/view\/9594.html
			js_loc, _ = js.Get("loc").String()

			//22 文件页数
			pageNumberStr, _ := js.Get("data").Get("url").Get("data").Get("display").Get("pageNumber").String()
			if len(pageNumberStr) <= 0 {
				continue
				fmt.Println("pageNumberStr")
			}

			//是一个数组 ，长度为10
			docImages := js.Get("data").Get("url").Get("data").Get("display").Get("docImage").MustStringArray()
			if docImages == nil {
				fmt.Println("docimages is error or empty")
				continue
			}

			//22
			*pageNumber, _ = strconv.Atoi(pageNumberStr)
			// fmt.Println("docimages length: ", len(docImages))
			// fmt.Println(" pagenumbers:  ", *pageNumber)
			// if len(docImages) != *pageNumber && len(docImages) != 10 {
			// 	fmt.Println("docimages length != pageNumber")
			// 	continue
			// }

			retry := 0
			fmt.Println("22222222222")
			for {
				issuccess := running(*redo, *pageNumber, *option, *docDownloadUrl, *imgDownloadUrl, *docId, js_loc, *docFormat, docImages)
				if issuccess {
					break
				}
				retry++
				if retry >= 1 {
					break
				}
			}
		}
	}
}
