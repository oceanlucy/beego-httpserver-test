package routers

import (
	"github.com/astaxie/beego"
	"ocean/controllers"
)

func init() {
	//用户就可以通过访问url“/ocean”（用户请求的地址） 去执行 OceanController 的逻辑
	beego.Router("/ocean", &controllers.OceanController{})
}
