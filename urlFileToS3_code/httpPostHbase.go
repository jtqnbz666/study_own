package main

import (
	"fmt"
	"io/ioutil"
	"math/rand"
	"net/http"
	"net/url"
)

var hbaseRand = [...]string{
	"http://10.173.18.77:17003",
	"http://10.173.18.95:17003",
	"http://10.174.32.7:17003",
	"http://10.173.236.101:17003",
	"http://10.174.47.228:17003",
	"http://10.174.47.210:17003",
	"http://10.174.47.243:17003",
	"http://10.173.236.119:17003",
	"http://10.174.32.8:17003",
	"http://10.174.32.25:17003",
	"http://10.174.32.26:17003",
	"http://10.174.32.43:17003",
	"http://10.174.32.44:17003",
	"http://10.173.18.77:17900",
	"http://10.173.18.95:17900",
	"http://10.174.32.7:17900",
	"http://10.173.236.101:17900",
	"http://10.174.47.228:17900",
	"http://10.174.47.210:17900",
	"http://10.174.47.243:17900",
	"http://10.173.236.119:17900",
	"http://10.174.32.8:17900",
	"http://10.174.32.25:17900",
	"http://10.174.32.26:17900",
	"http://10.174.32.43:17900",
	"http://10.174.32.44:17900",
	"http://10.173.18.77:20793",
	"http://10.173.18.95:20793",
	"http://10.174.32.7:20793",
	"http://10.173.236.101:20793",
	"http://10.174.47.228:20793",
	"http://10.174.47.210:20793",
	"http://10.174.47.243:20793",
	"http://10.173.236.119:20793",
	"http://10.174.32.8:20793",
	"http://10.174.32.25:20793",
	"http://10.174.32.26:20793",
	"http://10.174.32.43:20793",
	"http://10.174.32.44:20793",
}

func httpPostForm(table, row, group, col, value string) {
	resp, err := http.PostForm(hbaseRand[rand.Intn(len(hbaseRand))]+"/api/set_hbase_value",
		url.Values{"table": {table}, "row": {row}, "group": {group}, "col": {col}, "value": {value}})

	if err != nil {
		// handle error
		return
	}

	defer resp.Body.Close()
	body, err := ioutil.ReadAll(resp.Body)
	if err != nil {
		// handle error
	}

	fmt.Println(string(body))

}

func httpPostFormGet(table, row string) string {
	//table  ：  library_document_down_file
	//row    ：  md5(loc) = md5(https:\/\/www.xqppt.com\/view\/9594.html)
	resp, err := http.PostForm(hbaseRand[rand.Intn(len(hbaseRand))]+"/api/get_hbase_value",
		url.Values{"table": {table}, "key": {row}})

	if err != nil {
		// handle error
		return "{}"
	}

	defer resp.Body.Close()
	body, err := ioutil.ReadAll(resp.Body)
	if err != nil {
		// handle error
		return "{}"
	}

	return string(body)
}

func http_post_form(str string) string {
	resp, err := http.PostForm(str, nil)
	if err != nil {
		return "{}"
	}
	defer resp.Body.Close()
	body, err := ioutil.ReadAll(resp.Body)
	if err != nil {
		return "{}"
	}
	return string(body)
}
