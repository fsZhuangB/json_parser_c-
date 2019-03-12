# JSON语法子集表示方法

## 如下所示
```BNF
JSON-text = ws value ws
ws = *(%x20 / %x09 / %x0A / %x0D)
value = null / false /  true
null  = "null"
false = "false"
true  = "true"
```

## 解释
其中`%xhh`表示以16进制表示的字符，`/`是多选一，`*`是零个或多个，`()`用于分组。

1. 第一行意思即：JSON文本由三部分组成，首先是空白（whitespace），接着是一个值，最后是空白。
2. 第二行则告诉我们，所谓空白，是由零或者多个空格符（space U+0020）、制表符（tab U+0009）、换行符（LF U+000A）、回车符（CR U+000D）所组成。
3. 第三行则说：我们现时的值只可以是null、false或者true，他们都有对应的字面值（iteral）。
