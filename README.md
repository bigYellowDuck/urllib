# urllib

用C++模仿Python的urllib2库写到urlopen函数

用法：
auto resp = urllib::urlOpen("http://www.csdn.net");<br>
resp.read()     返回响应正文，即Html内容<br>
resp.info()     返回响应头部<br>
