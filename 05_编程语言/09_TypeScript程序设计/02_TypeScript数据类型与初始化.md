# TypeScript 数据类型与初始化

### 1. 类型总览

| 分类 | 类型 | 说明 |
| ---- | ---- | ---- |
| 原始类型 | `number`、`string`、`boolean` | 数字、字符串、布尔值 |
| 原始类型 | `undefined`、`null`、`symbol`、`bigint` | 特殊原始值 |
| 特殊类型 | `void`、`any`、`unknown`、`never` | 空值 / 任意 / 未知 / 永不 |
| 引用类型 | `object`、数组、元组（Tuple） | 对象及定长结构 |
| 引用类型 | `enum`、函数、`class` | 枚举、函数类型、类 |
| 高级类型 | 联合类型、字面量类型、`interface`、`type` | 组合与自定义类型 |

### 2. 原始类型（Primitive Types）

```typescript
// 数字类型（支持十进制、十六进制、二进制、八进制）
let num: number = 42;
let hex: number = 0xff;
let bin: number = 0b1010;
let oct: number = 0o744;
let float: number = 3.14;
let infinity: number = Infinity;
let notANum: number = NaN;

// 字符串类型
let str: string = "hello";
let template: string = `模板字符串：${str}，值为 ${num}`;

// 布尔类型
let flag: boolean = true;
let isDone: boolean = false;

// undefined 和 null（是其他类型的子类型，可赋给未开启 strictNullChecks 的任意类型）
let u: undefined = undefined;
let n: null = null;

// symbol 类型（生成唯一标识符）
let sym: symbol = Symbol("key");

// bigint 类型（超出 Number 安全范围的整数，需 target >= ES2020）
let big: bigint = 9007199254740991n;
let big2: bigint = BigInt("9007199254740991");
```

### 3. 引用类型（Reference Types）

```typescript
// 对象类型
let user: { name: string; age: number } = { name: "张三", age: 25 };
let obj: object = { key: "value" };

// 数组（两种等价写法）
let arr1: number[] = [1, 2, 3];
let arr2: Array<number> = [4, 5, 6];
let mixed: (string | number)[] = ["a", 1, "b"];
let emptyArr: string[] = []; // 空数组初始化

// 元组（Tuple）：固定长度，每个位置类型固定
let tuple: [string, number] = ["age", 25];
let optional: [string, number?] = ["only one"]; // 可选元素

// 枚举（enum）
enum Direction { Up, Down, Left, Right }          // 数字枚举，默认从 0 开始
enum Color { Red = 1, Green, Blue }                // 指定起始值
enum Status { Ok = "OK", Fail = "FAIL" }           // 字符串枚举
let dir: Direction = Direction.Up;                 // dir === 0
let color: Color = Color.Green;                    // color === 2
let st: Status = Status.Ok;                        // st === "OK"
```

### 4. 特殊类型

```typescript
// void：函数无返回值
function log(msg: string): void {
    console.log(msg);
}
let empty: void = undefined; // 只能赋 undefined（严格模式下）

// any：任意类型，跳过类型检查（尽量少用）
let anything: any = "str";
anything = 123;       // 不报错

// unknown：安全的 any，使用前必须做类型收窄
let data: unknown = fetchSomething();
if (typeof data === "string") {
    console.log(data.toUpperCase()); // 收窄后才能安全使用
}

// never：永不存在的值（抛出异常的函数、死循环、穷尽检查）
function throwError(msg: string): never {
    throw new Error(msg);
}
```

### 5. 函数类型

```typescript
// 完整声明：参数类型 + 返回值类型
function add(a: number, b: number): number {
    return a + b;
}

// 箭头函数
const multiply = (a: number, b: number): number => a * b;

// 可选参数（?）与默认参数（=）
function greet(name: string, greeting: string = "你好", title?: string): string {
    return `${greeting}, ${title ?? ""} ${name}`;
}

// 剩余参数
function sum(...nums: number[]): number {
    return nums.reduce((a, b) => a + b, 0);
}

// 函数类型变量
let fn: (x: number, y: number) => number = add;
```

### 6. class 类型

```typescript
class Person {
    // 属性声明与初始化（构造函数参数属性可简写）
    name: string;
    private age: number;
    readonly id: number = 0;

    constructor(name: string, age: number) {
        this.name = name;
        this.age = age;
    }

    getInfo(): string {
        return `${this.name}, ${this.age}`;
    }
}

let p: Person = new Person("李四", 30);

// 静态属性与继承
class Student extends Person {
    static count: number = 0;
    constructor(name: string, age: number, public grade: string) {
        super(name, age);
        Student.count++;
    }
}
```

### 7. 高级类型

```typescript
// 接口（interface）：描述对象形状
interface User {
    id: number;
    name: string;
    email?: string;      // 可选属性
    readonly createdAt: number; // 只读属性
}
let u1: User = { id: 1, name: "王五", createdAt: Date.now() };

// 类型别名（type）：可表示任意类型
type ID = number | string;
let uid: ID = 1001;
uid = "A-1001"; // 合法

// 联合类型（Union）
let value: string | number;
value = "abc";
value = 123;

// 字面量类型：值只能是固定的几个
let direction: "up" | "down" = "up";

// 类型断言：明确告知编译器具体类型
let someValue: unknown = "hello";
let len: number = (someValue as string).length;

// 类型推断：声明时不写类型，由初始值自动推断
let inferred = "abc";        // 推断为 string
inferred = 123;              // 报错：不能赋 number
```

### 8. 类型初始化要点小结

1. **先声明后赋值也必须有类型**：`let x: number; x = 1;` 合法；未注解且未初始化则推断为 `any`（`noImplicitAny` 开启时报错）
2. **类型推断优先**：有初始值时可省略类型注解，TS 自动推断，推荐简洁写法
3. **`const` 声明必须初始化**：`const x: number = 1;`，且推断为字面量类型
4. **可选属性 / 参数用 `?`**：`email?: string` 表示可以不存在
5. **只读用 `readonly`**：初始化后不可修改
6. **避免滥用 `any`**：无法确定类型时优先使用 `unknown`

