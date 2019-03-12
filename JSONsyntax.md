# JSON语法子集表示方法

## 如下所示
```ABNF
JSON-text = ws value ws
ws = *(%x20 / %x09 / %x0A / %x0D)
value = null / false /  true
null  = "null"
false = "false"
true  = "true"
```

## 解释
其中