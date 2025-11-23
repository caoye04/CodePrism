## 关联规范
- R_02_03: A project should not contain unused type declarations
- R_02_04: A project should not contain unused tag declarations
- R_02_05: A project should not contain unused macro declarations

## 违反说明
- 文件顶部定义的 `OtherType` 从未被引用，形成闲置的类型定义，与 R_02_03 要求相悖。
- `struct PhantomBlock` 仅声明未使用，属于多余的标签，触犯 R_02_04。
- 宏 `FLAG` 没有在任何地方展开，造成未使用的宏声明，违反 R_02_05。
