package main

import (
	. "gin_http/api"

	"github.com/gin-gonic/gin"
)

func initRouter() *gin.Engine {
	router := gin.Default()
	router.GET("/", IndexUsers) //http://192.168.2.132:8806

	//路由群组
	users := router.Group("api/v1/users")
	{
		users.GET("", GetAll)             //http://192.168.2.132:8806/api/v1/users
		users.POST("/add", AddUsers)      //http://192.168.2.132:8806/api/v1/users/add
		users.GET("/get/:id", GetOne)     //http://192.168.2.132:8806/api/v1/users/get/5
		users.POST("/update", UpdateUser) //http://192.168.2.132:8806/api/v1/users/update
		users.POST("/del", DelUser)       //http://192.168.2.132:8806/api/v1/users/del
	}

	departments := router.Group("api/v1/department")
	{
		departments.GET("", GetAll)             //http://192.168.2.132:8806/api/v1/users
		departments.POST("/add", AddUsers)      //http://192.168.2.132:8806/api/v1/users/add
		departments.GET("/get/:id", GetOne)     //http://192.168.2.132:8806/api/v1/users/get/5
		departments.POST("/update", UpdateUser) //http://192.168.2.132:8806/api/v1/users/update
		departments.POST("/del", DelUser)       //http://192.168.2.132:8806/api/v1/users/del
	}

	return router
}
