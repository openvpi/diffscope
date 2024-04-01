# C++ 编码风格

## Clang Format

文件编写完后，应当使用项目根目录的`.clang-format`格式化后再保存，以防出现不同编写者编写过后出现风格不一致。

## 头文件

### 一般引入规则

+ 引入顺序
    + 平台相关头文件，如`<Windows.h>`、`<sys/types.h>`
    + 标准库头文件，如`<cstdint>`、`<iostream>`
    + 按库依赖顺序从底层到高层，因此第三方库在本项目的库与可执行文件前；
    + 不同类型的头文件之间宜用空行分开，平台、标准库、第三方库的头文件之间必须分开，如以下
    ```c++
    #include <Windows.h>

    #include <iostream>
    #include <cstdint>

    #include <QtCore/QCoreApplication>
    #include <QtGui/QtEvents>

    #include "project.h"

    // ...
    ```

+ 没有依赖关系的库的头文件引入顺序可以任意，但是同一个库的应当集中，如
    ```c++
    #include <QtCore/QCoreApplication>
    #include <QtGui/QtEvents>

    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    ```

### 在源文件中引入头文件

+ 在`cpp`中，一般在最开始引入与之对应的头文件以及私有头文件，然后开始引入其他头文件，如以下
    ```c++
    #include "sample.h"
    #include "sample_p.h" // 如果有的话

    #include <iostream>
    
    #include <QtCore/QCoreApplication>
    ```

+ 不属于当前头文件所在项目的头文件，一律使用尖括号`<>`引入，否则使用引号`""`引入

## 接口风格

按照 Qt 代码风格，公有类应尽可能使用 pimpl 范式，如果是 Qt 的工程，则尽可能使用 Qt 风格。

+ 按照接口功能的相似度，适当使用换行
    ```c++
    // qcoreapplication.h
    static void setAttribute(Qt::ApplicationAttribute attribute, bool on = true);
    static bool testAttribute(Qt::ApplicationAttribute attribute);

    static void setOrganizationDomain(const QString &orgDomain);
    static QString organizationDomain();
    static void setOrganizationName(const QString &orgName);
    static QString organizationName();
    static void setApplicationName(const QString &application);
    static QString applicationName();
    static void setApplicationVersion(const QString &version);
    static QString applicationVersion();
    ```

+ 按照接口功能的相似度，适当添加`public`、`protected`及`private`关键字

+ getter 与 setter 的命名，getter 不添加`get`前缀，setter 添加`set`前缀
    ```c++
    // qdir.h
    QStringList nameFilters() const;
    void setNameFilters(const QStringList &nameFilters);
    ```

+ 类的声明中各组成部分顺序
    ```c++
    class Sample : public QObject {
        Q_OBJECT                    // Q_OBJECT
        Q_PROPERTY(...)             // Q_PROPERTY
        Q_DECLARE_PRIVATE(Sample)   // 其他宏
    public:
        explicit Sample(QObject *parent = nullptr); // 公有构造函数
        ~Sample(); // 公有析构函数
    
        // 公有接口
    public:
        int value() const;
        void setValue(int value);
    
    public:
        // ...
    
        // 信号
    Q_SIGNALS:
        void valueChanged(int value);

        // 公有槽
    public Q_SLOTS:
        void setText(const QString &text);

        // 保护成员函数等
    protected:
        virtual void initialize();
    
    protected:
        Sample(SamplePrivate &d, QObject *parent = nullptr);

        std::unique_ptr<SamplePrivate> d_ptr;

        // 友元函数
        friend class FriendClass1;

        // 禁用拷贝
        Q_DISABLE_COPY(Sample)
        
    protected:
        QString m_text;

        // 私有成员变量
    private:
        int m_value;

        // 私有槽函数
    private Q_SLOTS:
        void _q_textChanged(const QString &text);
    };
    ```

### 在头文件中引入其他头文件

+ 一般而言，在头文件中不应出现使用引号引入的其他头文件（即使是同一个目标中的），宜使用`<namespace/class.h>`形式引入，这对项目结构具有一定要求，如以下
    ```c++
    #include <QtCore/qglobal.h>

    #include <libavcodec/avcodec.h>
    ```

## C++ 相关

### 关键字

+ 在带有`Q_OBJECT`宏的`QObject`类中定义的，或在带有`Q_NAMESPACE`宏的命名空间中定义的枚举类型可使用`Q_ENUM`及`Q_ENUM_NS`来为枚举类型增加元数据，如字符串互转；

+ 不推荐使用`signals`、`slots`与`emit`，应使用`Q_SIGNALS`、`Q_SLOTS`与`Q_EMIT`；

+ 不使用`nodiscard`属性；

### 类型转换

+ 设计多态类时，应当定义一个列举派生类类型的枚举类型，或是在构造函数中强制让子类传入，或是定义一个纯虚函数强制子类实现。因此，在遇到一个基类指针时，应当尽可能减少`dynamic_cast`的使用，应判断类型后使用`static_cast`；

+ 同理，`qobject_cast`也应当减少使用。尤其是在收到一个实例的`QObject::destroyed()`信号进行后续处理时，信号发送者已经析构到`QObject`类，此时使用`qobject_cast`回派生类将会失败；

### 符号声明

+ 根据需要适当使用匿名命名空间、静态全局函数以及内联函数；

+ 单例适合作为静态全局变量；

### Qt 特性

+ Qt 容器一般自带写时复制的优化，在使用非`const`方法访问后会隐式复制一份，但在 C++ 11 的 for 循环语法糖中，被遍历的容器是否是以`const`访问是不确定的，此时应使用`qAsConst`将容器强制为不可写，如果使用了 C++ 17，应当使用`std::as_const`。另外，`std::as_const`在非 Qt 项目中也应该按需使用；

+ 由于 Qt 容器带有写时复制优化，当函数返回 Qt 容器时，不需要以常引用形式；

+ 继承自`QObject`的类，应当为其定义一个传入`parent`的构造函数，与 Qt 保持统一；

+ 如果使用`QObject`不需要用到元对象特性，可以不添加`Q_OBJECT`宏，尤其是在`cpp`中内部使用时（以地址的方式连接槽函数时也不需要，除非使用的是信号）；

+ 如果需要使用一部分元对象特性，但不需要使用信号槽等功能，可以使用`Q_GADGET`；

+ 在一个`QObject`需要同时被多处引用时，可以按需使用`QPointer`，它可以保证当对象析构后自动清空，避免野指针；

+ `QString`字面值适合使用`QStringLiteral`包装，减少运行时内存分配；

+ 在类的成员中，尽可能少地使用 C 风格的数组，使用`std::array`代替；

### 前置知识

+ 标准库数据结构
+ 编译、链接（重定位）
+ 虚函数表
+ 写时复制

### 一个规范的库结构

```
+ build
    + include
        + mylib
              xxx.h
              ...
    + etc/include
          mylib_config.h
+ docs
+ examples
+ scripts
+ share
+ src
    + module1
        + core
        + internal
        + ...
          mylib_global.h
          CMakeLists.txt
    + module2
      CMakeLists.txt
      mylibConfig.cmake.in
+ tests
  CMakeLists.txt
```
使用`qmsetup`辅助构建。