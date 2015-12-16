package main

import (
	"github.com/astaxie/beego"
	_ "ocean/routers"
)

func main() {
	//beego全局变量
	beego.CopyRequestBody = true
	beego.AppName = "ocean"

	//设置http端口号
	beego.HttpAddr = "127.0.0.1"
	//设置http端口号
	beego.HttpPort = 5170

	//无参数，默认default run on HttpPort
	beego.Run()
}
