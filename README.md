# urllib

用C++模仿Python的urllib2库写到urlopen函数

用法：
auto resp = urllib::openUrl("http://www.csdn.net");

resp.read()     返回响应正文，即Html内容
resp.info()     返回响应头部
