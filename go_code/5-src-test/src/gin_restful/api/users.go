package api

import (
	"fmt"
	. "gin_http/models"
	"net/http"
	"strconv"

	"github.com/gin-gonic/gin"
)

// index
func IndexUsers(c *gin.Context) {
	c.String(http.StatusOK, "It works")
}

// 增加一条记录
func AddUsers(c *gin.Context) {
	name := c.Request.FormValue("name")
	telephone := c.Request.FormValue("telephone")
	fmt.Println("name:", name)
	fmt.Println("telephone:", telephone)
	if name == "" {
		msg := fmt.Sprintf("name字段错误")
		c.JSON(http.StatusBadRequest, gin.H{
			"msg": msg,
		})
		return
	}
	person := Person{
		Name:      name,
		Telephone: telephone,
	}
	id := person.Create()
	msg := fmt.Sprintf("insert 成功 %d", id)
	c.JSON(http.StatusOK, gin.H{
		"msg": msg,
	})
}

// 获得一条记录
func GetOne(c *gin.Context) {
	ids := c.Param("id")
	id, _ := strconv.Atoi(ids)
	p := Person{
		Id: id,
	}
	rs, _ := p.GetRow()
	c.JSON(http.StatusOK, gin.H{
		"result": rs,
	})
}

// 获得所有记录
func GetAll(c *gin.Context) {
	p := Person{}
	rs, _ := p.GetRows()
	c.JSON(http.StatusOK, gin.H{
		"list": rs,
	})
}

func UpdateUser(c *gin.Context) {
	ids := c.Request.FormValue("id")
	id, _ := strconv.Atoi(ids)
	telephone := c.Request.FormValue("telephone")
	person := Person{
		Id:        id,
		Telephone: telephone,
	}
	row := person.Update()
	msg := fmt.Sprintf("updated successful %d", row)
	c.JSON(http.StatusOK, gin.H{
		"msg": msg,
	})
}

// 删除一条记录
func DelUser(c *gin.Context) {
	ids := c.Request.FormValue("id")
	id, _ := strconv.Atoi(ids)
	row := Delete(id)
	msg := fmt.Sprintf("delete successful %d", row)
	c.JSON(http.StatusOK, gin.H{
		"msg": msg,
	})
}
