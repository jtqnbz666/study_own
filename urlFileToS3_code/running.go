package main

import (
	"bufio"
	"crypto/sha1"
	"encoding/hex"
	"encoding/json"
	"fmt"
	"io"
	"io/ioutil"
	"math/rand"
	"net/http"
	"os"
	"regexp"
	"strconv"
	"strings"
	"sync"
	"time"

	"gitee.com/fengling/gotools/simplejson"
)

var wg sync.WaitGroup //定义一个同步等待的组

func SHA1(s string) string {

	o := sha1.New()

	o.Write([]byte(s))

	return hex.EncodeToString(o.Sum(nil))

}

func RepImages(htmls string) string {
	var imgRE = regexp.MustCompile(`<img[^>]+\bsrc=["']([^"']+)["']`)
	imgs := imgRE.FindAllStringSubmatch(htmls, -1)
	fmt.Println(imgs)
	if len(imgs) > 0 {
		return imgs[0][1]
	}
	return ""
}

func RepDocUrl(htmls string) string {
	resJson, err := simplejson.NewJson([]byte(htmls))
	if err != nil {
		fmt.Println(err.Error())
		return ""
	}
	resLink, err := resJson.Get("link").String()
	if err == nil && len(resLink) > 0 {
		return resLink
	}
	return ""
}

func getFileSize(filename string) (int64, error) {
	fi, err := os.Stat(filename)
	if err != nil {
		return 0, err
	}
	return fi.Size(), nil
}

func DownloadFile(real_docDownloadUrl, filename, s3_bucket string) {
	client := &http.Client{}

	req, err := http.NewRequest(http.MethodGet, real_docDownloadUrl, nil)
	if err != nil {
		fmt.Println("文档下载失败 1")
		wg.Done()
		return
	}
	resp, err := client.Do(req)
	if err != nil {
		fmt.Println("文档下载失败 2")
		wg.Done()
		return
	}

	resCode := resp.StatusCode
	if resCode != 200 {
		defer resp.Body.Close()
		fmt.Println("rescode: ", resCode, "downloadUrl: ", real_docDownloadUrl)
		wg.Done()
		return
	}
	defer resp.Body.Close()
	body, err := ioutil.ReadAll(resp.Body)
	fmt.Println("first docurl response: " + string(body))
	//innerDocDownloadUrl := RepDocUrl(string(body))
	innerDocDownloadUrl := string(body)
	//innerDocDownloadUrl = strings.ReplaceAll(innerDocDownloadUrl, " ", "%20")
	//fmt.Println("innerDocDownloadUrl: " + innerDocDownloadUrl)

	directlyDown(client, innerDocDownloadUrl, filename, s3_bucket)
}
func directlyDown(client *http.Client, innerDocDownloadUrl, filename, s3_bucket string) {
	innerDocDownloadUrl = strings.ReplaceAll(innerDocDownloadUrl, " ", "%20")
	fmt.Println("innerDocDownloadUrl: " + innerDocDownloadUrl)
	req, err := http.NewRequest(http.MethodGet, string(innerDocDownloadUrl), nil)
	if err != nil {
		fmt.Println("文档下载失败 3")
		wg.Done()
		return
	}
	req.Header.Add("Host", "down.qiqiwenku.com")

	// client.CheckRedirect = func(req *http.Request, via []*http.Request) error {
	// 	if len(via) >= 1 {
	// 		return fmt.Errorf("first response")
	// 	}
	// 	return nil
	// }
	resp, err := client.Do(req)

	if err != nil {
		fmt.Println("文档下载失败 4", err)
		wg.Done()
		return
	}
	defer resp.Body.Close()

	file, err := os.OpenFile(filename, os.O_CREATE|os.O_WRONLY, 0644)
	defer file.Close()
	f := bufio.NewWriter(file)
	io.Copy(f, resp.Body)
	f.Flush()
	sends3(filename, s3_bucket)
	wg.Done()
}

func DownloadImg(img_url, filename_img string) {
	// 下载图片
	retry := 0
	retry_max := 10
	for {
		retry++
		if retry > retry_max {
			fmt.Println("img get failed exceed times, url = " + img_url)
			break
		}
		resp, err := http.Get(img_url)
		if err != nil {
			fmt.Println("image download failed, img_url = " + img_url)
			//defer resp.Body.Close()
			continue
		}
		resCode := resp.StatusCode
		if resCode != 200 {
			fmt.Println("rescode: ", resCode, " img_url = "+img_url)
			defer resp.Body.Close()
			continue
		}
		defer resp.Body.Close()
		file, err := os.OpenFile(filename_img, os.O_CREATE|os.O_WRONLY, 0644)
		defer file.Close()
		f := bufio.NewWriter(file)
		io.Copy(f, resp.Body)
		f.Flush()
		break
	}
	wg.Done()
}

func sendTuChuang(filename_img string) string {
	retry := 0
	MAX_RETRY := 3
	for {
		time.Sleep(time.Duration(100) * time.Millisecond)
		retry++
		if retry > MAX_RETRY {
			fmt.Println("retry times is exceed, imgfilename = " + filename_img)
			break
		}

		tuChuangRes := sendTuchuang(filename_img)
		if len(tuChuangRes) > 0 {
			jsTuChuang, err := simplejson.NewJson([]byte(tuChuangRes))
			if err != nil {
				fmt.Println("tuchuangres is not json")
				continue
			}
			js0 := jsTuChuang.GetIndex(0)
			ERRORMSG, _ := js0.Get("ERRORMSG").String()
			if ERRORMSG != "success" {
				fmt.Println("ERRORMSG not success, imgfilename = " + filename_img)
				continue
			} else {
				return tuChuangRes
			}
		} else {
			fmt.Println("tuchuangres is empty")
		}
	}
	return ""
}

func running(reDo, pageNumber int, option, docDownloadUrl, imgDownloadUrl, id, loc, docFormat string, docImages []string) bool {
	//secret := []byte("89DFFFC985B0CB32")
	var secret []byte
	if option == "tgppt" {
		secret = []byte("oorJzRC6B&HGZ!*KXG6d")
	} else {
		secret = []byte("5B0CB3289DFFFC98")
	}

	timeUnix := time.Now().Unix()
	timeUnixStr := strconv.FormatInt(timeUnix, 10)
	fmt.Println(timeUnixStr)
	timeAndSecret := timeUnixStr + string(secret)
	sign := SHA1(md5sum(timeAndSecret)) //签名
	//https://www.qiqiwenku.com/api/source-file/download?op=download&url={}&id={}&time=112312&sign=95380101be90af46adde2f71875488f30d493cf5
	docDownloadUrlFormat := strings.ReplaceAll(docDownloadUrl, "{}", "%s")

	//https://www.qiqiwenku.com/api/source-file/download?op=download&url=https:\/\/www.xqppt.com\/view\/9594.html&id=9594&time=112312&sign=95380101be90af46adde2f71875488f30d493cf5
	real_docDownloadUrl := fmt.Sprintf(docDownloadUrlFormat, "download", loc, id, timeUnixStr, sign)

	fmt.Println("timeAndSecret: " + timeAndSecret)
	fmt.Println("sign: " + sign)
	fmt.Println("read_docDownloadUrl: " + real_docDownloadUrl)

	//loc 为 https:\/\/www.xqppt.com\/view\/9594.html
	row := md5sum(loc)
	fmt.Println("row: " + row)
	hbaseRes := httpPostFormGet("library_document_down_file", row)
	if len(hbaseRes) <= 0 {
		hbaseRes = "{}"
	}
	jsGet, _ := simplejson.NewJson([]byte(hbaseRes))
	hbaseJson, _ := jsGet.Get("data").Get("content:json").String()
	if len(hbaseJson) <= 0 {
		hbaseJson = "{}"
	}
	jsHbaseRes, _ := simplejson.NewJson([]byte(hbaseJson))

	hbaseTuchuangKey, _ := jsHbaseRes.Get("tuchuang_key").String()
	filename, _ := jsHbaseRes.Get("s3_path").String()
	s3_bucket, _ := jsHbaseRes.Get("s3_bucket").String()
	hbaseFileMd5, _ := jsHbaseRes.Get("file + _md5").String()

	filename = "suda_" + row + "." + docFormat
	origin2, _ := strconv.ParseInt(row[0:4], 16, 64)
	s3_bucket = "chazidian-wenku-" + strconv.FormatInt(origin2%21, 10)
	fmt.Println("s3_bucket: " + s3_bucket)

	if reDo == 0 && len(hbaseTuchuangKey) > 0 && hbaseTuchuangKey != "null" {
		fmt.Println("该文件获取，不重新拉取")
		//continue
		return true
	}

	wg.Add(1)
	//loc

	if option == "tgppt" {
		go directlyDown(&http.Client{}, real_docDownloadUrl, filename, s3_bucket)
	} else {
		go DownloadFile(real_docDownloadUrl, filename, s3_bucket)
	}

	imgDownloadUrlFormat := strings.ReplaceAll(imgDownloadUrl, "{}", "%s")
	var tuchuangUrl []string
	tuchangOK := false

	for i := 1; i <= pageNumber && i <= 10; i++ {
		var real_imgDownloadUrl string
		if option == "tgppt" {
			real_imgDownloadUrl = fmt.Sprintf(imgDownloadUrlFormat, loc, id, strconv.Itoa(i), timeUnixStr)
		} else {
			real_imgDownloadUrl = fmt.Sprintf(imgDownloadUrlFormat, loc, id, strconv.Itoa(i))
		}
		fmt.Println("read_imgDownloadUrl: " + real_imgDownloadUrl)

		filename_img := row + fmt.Sprintf("_%v", i) + ".jpeg"
		//js, _ := simplejson.NewJson([]byte(in[1]))
		//fmt.Println("拉取图片文件:" + filename_img)
		//img_url := real_imgDownloadUrl
		//img_url := docImages[i-1]
		//js_resp, _ := simplejson.NewJson([]byte(http_post_form(real_imgDownloadUrl)))

		//img_url, _ := js_resp.Get("data").String()
		img_url := real_imgDownloadUrl
		fmt.Println("cur img_url: " + img_url)
		wg.Add(1)
		go DownloadImg(img_url, filename_img) //先把图片下载到当前目录下
	}

	wg.Wait()
	for i := 1; i <= pageNumber && i <= 10; i++ {
		//real_imgDownloadUrl := fmt.Sprintf(imgDownloadUrlFormat, id, strconv.Itoa(i))
		//fmt.Println("read_imgDownloadUrl: " + real_imgDownloadUrl)

		filename_img := row + fmt.Sprintf("_%v", i) + ".jpeg"
		//fmt.Println("拉取图片文件:" + filename_img)
		//img_url := real_imgDownloadUrl

		tuChuangRes := ""
		filesize_img, _ := getFileSize(filename_img)
		fmt.Println("filesize: ", filesize_img)
		if filesize_img == 0 {
			fmt.Println("image download failed , file= " + filename_img)
			break
		}
		tuChuangRes = sendTuChuang(filename_img) //把刚才下载下来的图片上传到图床上去。
		os.Remove(filename_img)                  //移除刚才下载在本地的图片
		//fmt.Println("tuChuangRes" + tuChuangRes)//打印出来就是下边这些
		//tuChuangRes[{"STATUS":"SUCC","ERRORMSG":"success","ERRORNO":0,"DATA":{"demo":{"KB":"55.4","SIZE":"1080x608","FORMAT":"JPEG","QUALITY":75,"FRAMENO":1,"KEY":"t01c534c63025d59a9d","URL":["http:\/\/p0.qhimg.com\/t01c534c63025d59a9d.jpg","http:\/\/p1.qhimg.com\/t01c534c63025d59a9d.jpg","http:\/\/p2.qhimg.com\/t01c534c63025d59a9d.jpg","http:\/\/p3.qhimg.com\/t01c534c63025d59a9d.jpg","http:\/\/p4.qhimg.com\/t01c534c63025d59a9d.jpg","http:\/\/p5.qhimg.com\/t01c534c63025d59a9d.jpg","http:\/\/p6.qhimg.com\/t01c534c63025d59a9d.jpg","http:\/\/p7.qhimg.com\/t01c534c63025d59a9d.jpg","http:\/\/p8.qhimg.com\/t01c534c63025d59a9d.jpg","http:\/\/p9.qhimg.com\/t01c534c63025d59a9d.jpg"],"SAVE_FLOW_KB":"2.2\/57.6","IP":"127.0.0.1"}}}]
		if len(tuChuangRes) > 0 {
			jsTuChuang, err := simplejson.NewJson([]byte(tuChuangRes))
			if err != nil {
				fmt.Println("tuChuangRes return false, file = " + filename_img)
				tuchangOK = false
				break
			}
			js0 := jsTuChuang.GetIndex(0) //因为得到的是一个json数组,所以取第一个位置
			ERRORMSG, _ := js0.Get("ERRORMSG").String()
			if ERRORMSG == "success" {
				tuchangOK = true
			} else {
				// 顺次获取图片 ，遇到失败则退出。 抓几张展示几张
				tuchangOK = true
				fmt.Println("ERRORMSG not success")
				break
			}
			tuchangKey, _ := js0.Get("DATA").Get("demo").Get("KEY").String()

			var _tuChuangRes []map[string]interface{}
			err = json.Unmarshal([]byte(tuChuangRes), &_tuChuangRes)
			if err != nil {
				fmt.Println("json.Unmarshal failed , file = " + filename_img)
				tuchangOK = false
				break
			}

			//fmt.Println(tuchangOK, tuchangKey)
			if tuchangOK && len(tuchangKey) > 0 {
				rand.Intn(10)
				tuchuangUrl = append(tuchuangUrl, "https://p"+strconv.Itoa(rand.Intn(10))+".qhimg.com/"+tuchangKey+".png")
			} else {
				fmt.Println("tuchangOk is false or len(tuchangKey <= 0)" + tuChuangRes)
			}
		}
	}

	if len(tuchuangUrl) < 10-1 && len(tuchuangUrl) < pageNumber-1 {
		fmt.Println("images download inadequate. error_download_failed:###" + loc + "###")
		os.Remove(filename)
		return false
	}
	if len(tuchuangUrl) > 0 {
		date := time.Now().Format("20060102150405")
		fmt.Println(date) //打印一个时间戳
		whbaseJson := "{}"
		wjsHbaseRes, _ := simplejson.NewJson([]byte(whbaseJson))

		wjsHbaseRes.SetPath([]string{"pic_from"}, "third")
		wjsHbaseRes.SetPath([]string{"s3_bucket"}, s3_bucket)
		wjsHbaseRes.SetPath([]string{"s3_path"}, filename)
		file_md5, _ := CalcFileMD5(filename) //将文件内容做一个md5
		if len(file_md5) == 0 {
			file_md5 = hbaseFileMd5
		}
		wjsHbaseRes.SetPath([]string{"file_md5"}, file_md5)
		wjsHbaseRes.SetPath([]string{"js_loc"}, loc)

		tuchuangUrlJson, err := json.Marshal(tuchuangUrl) //是一个图片链接的数组,将字符串转化为json的格式
		if err != nil {
			io.WriteString(os.Stderr, "json marshal failed,err:"+err.Error()+"\n")
			fmt.Println("json.Marshal tuchuangUrl . error_download_failed:###" + loc + "###")
			os.Remove(filename)
			return false
		}
		wjsHbaseRes.SetPath([]string{"tuchuang_key"}, string(tuchuangUrlJson))

		whbaseValue, err := json.Marshal(wjsHbaseRes)
		if err != nil {
			io.WriteString(os.Stderr, "json marshal failed,err:"+err.Error()+"\n")
			os.Remove(filename)
			fmt.Println("json.Marshal wjsHbaseRes. error_download_failed:###" + loc + "###")
			return false
		}
		fmt.Println(row + "\t" + string(whbaseValue)) //
		//添加check 检查S3中是否有数据

		filesize, _ := getFileSize(filename)
		fmt.Println("filesize: ", filesize)
		if filesize < 300 {
			os.Remove(filename)
			fmt.Println("filesize < 300 .error_download_failed: ###" + loc + "###")
			return false
		}
		isSuccess := getfroms3(filename, s3_bucket) //这里面会打印HTTP的reqeust和response的详细信息
		//下边这两个操作, 就是去写hbase
		if isSuccess {
			httpPostForm("library_document_down_file", row, "content", "json", string(whbaseValue))
			//并发高会失败
			httpPostForm("library_document_down_file", row, "content", "json_"+date, string(whbaseValue))
			fmt.Println("error_download_success:\t" + loc)
		}

	} else {
		fmt.Println("tuchuangurl < 0 . error_download_failed: ###" + loc + "###")
	}
	os.Remove(filename)
	return true
}
