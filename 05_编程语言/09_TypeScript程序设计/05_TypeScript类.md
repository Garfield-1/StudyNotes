

# TypeScript 类（class）

## 类的定义与实例化

类是对象的模板：声明属性与类型、在构造函数中初始化、定义方法，通过 `new` 创建实例。

```typescript
class Person {
    name: string;  // 属性必须先声明类型
    age: number;

    constructor(name: string, age: number) {
        this.name = name;
        this.age = age;
    }

    // 方法注解返回值类型
    getInfo(): string {
        return `${this.name}, ${this.age}岁`;
    }
}

// 实例的类型自动推断为 Person
const p = new Person("张三", 25);
console.log(p.getInfo()); // "张三, 25岁"
```

## 构造函数与参数属性简写

在构造函数参数前加修饰符，可自动完成"声明 + 初始化"，省去重复的 `this.xxx = xxx`。

```typescript
class User {
    // 参数属性简写：public name 自动成为实例属性
    constructor(
        public name: string,
        public age: number,
        private password: string // 私有属性，外部不可访问
    ) {}
}

const u = new User("李四", 30, "123456");
console.log(u.name);      // ✅ "李四"
// console.log(u.password); // ❌ 报错：password 是私有的
```

## 继承（extends）与 super

子类用 `extends` 继承父类，获得父类的属性与方法；构造函数中必须先调用 `super()` 才能使用 `this`。

```typescript
class Animal {
    constructor(public name: string) {}

    move(distance: number): void {
        console.log(`${this.name} 移动了 ${distance} 米`);
    }
}

class Dog extends Animal {
    constructor(name: string, public breed: string) {
        super(name); // 必须调用父类构造函数
        this.breed = breed;
    }

    bark(): void {
        console.log("汪汪!");
    }
}

const dog = new Dog("旺财", "柴犬");
dog.move(10); // 继承自 Animal
dog.bark();   // Dog 自己的方法
```

## 方法重写（override）

子类可重写父类方法，用 `super.方法名()` 调用父类原实现；开启 `noImplicitOverride` 时需显式加 `override` 关键字。

```typescript
class Base {
    greet(): string {
        return "你好";
    }
}

class Child extends Base {
    override greet(): string {
        return `${super.greet()}，我是子类`; // 调用父类原实现
    }
}

new Child().greet(); // "你好，我是子类"
```

## 访问修饰符：public / private / protected

控制属性与方法在类内、子类、外部的可见性。

```typescript
class Account {
    public owner: string;     // 公开：任何地方可访问（默认值，可省略）
    private balance: number;  // 私有：仅类内部可访问
    protected code: string;   // 受保护：类内部与子类可访问，外部不可

    constructor(owner: string, balance: number, code: string) {
        this.owner = owner;
        this.balance = balance;
        this.code = code;
    }

    private check(): boolean {
        return this.balance > 0; // ✅ 类内部可访问私有成员
    }

    deposit(amount: number): void {
        this.balance += amount;
    }
}

class VipAccount extends Account {
    showCode(): string {
        return this.code; // ✅ 子类可访问 protected 成员
        // return this.balance; // ❌ 报错：balance 是 private
    }
}

const acc = new Account("王五", 100, "A01");
console.log(acc.owner);   // ✅ public
// console.log(acc.balance); // ❌ 报错：private
// console.log(acc.code);    // ❌ 报错：protected
```

> 补充：TS 3.8 起还支持 `#` 原生私有字段（如 `#secret: string`），是 JS 运行时的真私有，而 `private` 仅是编译期检查。

## readonly 只读修饰符

`readonly` 属性只能在声明处或构造函数中赋值，之后不可修改；可与访问修饰符组合使用。

```typescript
class Config {
    readonly version: string = "1.0";           // 声明时初始化
    readonly createdAt: number;                  // 构造函数中初始化
    private readonly secret: string = "key";     // 只读 + 私有

    constructor(createdAt: number) {
        this.createdAt = createdAt; // ✅ 仅此处可赋值
        // this.version = "2.0";    // ❌ 报错：只读属性不可修改
    }
}
```

## static 静态成员

静态成员属于类本身而非实例，通过 `类名.成员` 访问，不能在实例上访问。

```typescript
class Counter {
    static count: number = 0; // 静态属性，所有实例共享

    constructor() {
        Counter.count++; // 类内部也要用类名访问静态成员
    }

    static reset(): void {
        Counter.count = 0;
    }
}

new Counter();
new Counter();
console.log(Counter.count); // 2
Counter.reset();
```

## 抽象类（abstract）

抽象类不能被直接实例化，只能作为基类被继承；抽象方法只有签名没有实现，**子类必须实现**，适合定义公共骨架与强制约束。

```typescript
abstract class Shape {
    abstract area(): number; // 抽象方法：子类必须实现

    // 抽象类也可以有普通方法与已实现逻辑
    describe(): string {
        return `面积是 ${this.area()}`;
    }
}

// const s = new Shape(); // ❌ 报错：抽象类不能实例化

class Circle extends Shape {
    constructor(private radius: number) {
        super();
    }

    area(): number { // 实现抽象方法
        return Math.PI * this.radius ** 2;
    }
}

const c = new Circle(2);
console.log(c.describe()); // "面积是 12.566..."
```

## 访问修饰符对比表

| 修饰符           | 类内部         | 子类         | 外部（实例） | 典型用途                    |
| ---------------- | -------------- | ------------ | ------------ | --------------------------- |
| `public`（默认） | ✅              | ✅            | ✅            | 对外暴露的 API              |
| `protected`      | ✅              | ✅            | ❌            | 只给子类用的内部逻辑        |
| `private`        | ✅              | ❌            | ❌            | 类的内部实现细节            |
| `#` 原生私有     | ✅              | ❌            | ❌            | 需要运行时真私有的场景      |
| `readonly`       | 仅初始化时写入 | —            | 只读         | 不可变配置 / 创建时间等     |
| `static`         | 通过类名访问   | 通过类名访问 | 通过类名访问 | 实例间共享的计数 / 工具方法 |

