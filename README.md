# rafa_json
A good helper for you to parse json!
## why this name
I hope this tool will **rapid**, **fast** to parse json!


## 系统的主要设想

在我的JSON paser中，我希望实现的功能有：

- 符合[标准](http://www.ecma-international.org/publications/files/ECMA-ST/ECMA-404.pdf)的JSON解析器和生成器，包含
  1. 能够完整解析JSON标准中提到的JSON的6种数据类型，即null、boolean、number、string、array、object。也就是能将JSON文本解析成一个树状数据结构(parse)。
  2. 提供封装良好的接口去访问该数据结构(access)。
  3. 把数据结构转化成JSON文本(stringfy)。
  如图所示：![](http://wx3.sinaimg.cn/mw690/be620060ly1g0zuo095aej20et04jt8r.jpg)

## end
