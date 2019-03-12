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