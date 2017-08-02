数据上报模块，该模块是独立模块，通过http上传到指定地址，上传机制是，数据采集先保存在文件save.txt，定时上传时间到，
先把save.txt复制到report.txt，然后情况save.txt，然后再把report.txt数据上传到服务器，如果上传成功，把文件里的数据清空

主要功能代码模块：
1、数据上报http模块
2、文件管理fileHandle
3、数据处理dataHandle
4、网络请求处理http
5、测试程序入口dataReport
6、md5加密机制
7、定时上传