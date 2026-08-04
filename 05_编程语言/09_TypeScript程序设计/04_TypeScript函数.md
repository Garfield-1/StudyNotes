# TypeScript函数

## TypeScript函数的定义方法

## 1. 函数声明（Function Declaration）

最常用的方式，参数类型与返回值类型都显式注解。

```typescript
function add(a: number, b: number): number {
    return a + b;
}
```

### 2. 函数表达式（Function Expression）

用变量保存一个匿名函数，变量本身不写类型，由右侧函数推断。

```typescript
const subtract = function (a: number, b: number): number {
    return a - b;
};
```

### 3. 箭头函数（Arrow Function）

`ES6` 语法，没有自己的 `this`，适合回调与简短逻辑。

```typescript
const multiply = (a: number, b: number): number => a * b;
const square = (x: number) => x * x; // 单参数可省略括号，返回值可自动推断
```

### 4. 函数类型注解

先定义函数类型，再约束变量；类型中的参数名仅作语义说明，不参与类型检查。

```typescript
let calc: (x: number, y: number) => number;
calc = add;
calc = subtract;
```

### 5. 用 interface / type 描述函数类型

适合复杂签名、需要携带属性的函数；被注解的实现可获得参数类型的上下文推断。

```typescript
interface MathFn {
    (a: number, b: number): number; // 调用签名
    description?: string;           // 函数对象还可携带属性
}
type BinaryOp = (a: number, b: number) => number;

const divide: BinaryOp = (a, b) => a / b; // 参数类型由 BinaryOp 上下文推断
```

### 6. 构造函数（通过 new 调用）

用伪参数 `this` 注解构造函数内部 `this` 的类型。

```typescript
function UserFactory(this: { name: string }, name: string) {
    this.name = name;
}
```

### 7. 泛型函数（Generic Function）

类型参数化，保留入参与返回值之间的类型关联。

```typescript
function identity<T>(value: T): T {
    return value;
}
identity<string>("abc"); // 显式指定
identity(123);           // 自动推断 T 为 number
```

### 8. 函数重载（Function Overloads）

同名函数根据参数类型给出不同的签名，最后一个是实现签名（不对外可见）。

```typescript
function format(value: number): string;
function format(value: Date): string;
function format(value: number | Date): string {
    return value instanceof Date ? value.toISOString() : String(value);
}
```

> 注意：匿名函数（立即执行函数 IIFE）同样适用，如 `(function (x: number): void { console.log(x); })(1);`。

## TypeScript函数参数传递方法

### 1. 参数类型注解规则

```typescript
// 必选参数：调用时数量与类型都必须匹配
function greet(name: string): void { /* ... */ }

// 可选参数（?）：可不传，类型自动包含 undefined；必须位于必选参数之后
function buildUrl(path: string, query?: string): string {
    return query ? `${path}?${query}` : path;
}

// 默认参数（=）：有默认值的参数自动变为可选，可位于任意位置（但建议放后面）
function pow(base: number, exp: number = 2): number {
    return base ** exp;
}
pow(3);     // 9，exp 取默认值 2
pow(2, 10); // 1024

// 剩余参数（Rest Parameters）：收集不定数量参数，必须是最后一个参数
function sum(...nums: number[]): number {
    return nums.reduce((a, b) => a + b, 0);
}
sum(1, 2, 3, 4); // 10
```

### 2. 对象解构参数（推荐用于多参数场景）

```typescript
// 用对象作为单个参数，配合解构 + interface，可读性与可维护性最佳
interface CreateUserOptions {
    name: string;
    age: number;
    email?: string;
    isAdmin?: boolean; // 可选配置项
}

function createUser({ name, age, email = "", isAdmin = false }: CreateUserOptions): void {
    console.log(name, age, email, isAdmin);
}

createUser({ name: "张三", age: 25 });                        // 只传必选
createUser({ name: "李四", age: 30, email: "a@b.com" });      // 部分可选
createUser({ name: "王五", age: 20, isAdmin: true });

// 整个解构对象可选（参数本身可不传）
function connect({ host = "localhost", port = 3000 }: { host?: string; port?: number } = {}): void {
    console.log(`${host}:${port}`);
}
connect();                 // localhost:3000
connect({ port: 8080 });   // localhost:8080
```

### 3. 参数传递机制（值传递 vs 引用传递）

`TypeScript`（同 `JavaScript`）**没有 C++ 那样的"引用传递"语法**，统一是**按值传递**，但传递的内容本身可能是引用：

| 参数类型                                     | 传递内容                   | 函数内修改对外部的影响                                       |
| -------------------------------------------- | -------------------------- | ------------------------------------------------------------ |
| 原始类型（`number`、`string`、`boolean` 等） | 值的副本                   | 无影响                                                       |
| 对象 / 数组 / 函数（引用类型）               | 引用的副本（指向同一对象） | 修改对象属性 / 数组元素会影响外部；但**重新赋值参数本身不影响外部** |

```typescript
// 原始类型：修改的是副本
function changeNum(x: number): void {
    x = 100; // 不影响外部变量
}
let n = 1;
changeNum(n); // n 仍为 1

// 引用类型：共享同一对象，修改属性会影响外部
function rename(user: { name: string }): void {
    user.name = "新名字";      // ✅ 影响外部对象
    user = { name: "别人" };   // ❌ 只修改了局部引用，不影响外部变量
}

// 若想避免函数意外修改对象，可用 readonly / Readonly<T>
function readUser(user: Readonly<{ name: string; age: number }>): void {
    // user.name = "x"; // 报错：只读属性不可修改
}
```

## TypeScript函数返回值类型

### 1. 返回值注解与推断

```typescript
// 显式注解返回值类型
function add(a: number, b: number): number {
    return a + b;
}

// 省略注解时由 return 语句自动推断
function concat(a: string, b: string) {
    return a + b; // 推断返回 string
}
```

### 2. 常见的返回值类型

| 返回值类型   | 写法                                | 说明                                     |
| ------------ | ----------------------------------- | ---------------------------------------- |
| 普通类型     | `: number` / `: string` 等          | 返回单一确定类型                         |
| 对象类型     | `: { id: number; name: string }`    | 返回对象字面量                           |
| 数组 / 元组  | `: number[]` / `: [number, string]` | 返回列表或定长结构                       |
| 联合类型     | `: string \| number`                | 可能返回多种类型                         |
| 字面量类型   | `: "success" \| "fail"`             | 返回值只能是固定的几个                   |
| `void`       | `: void`                            | 无返回值（或 `return;` 不带值）          |
| `undefined`  | `: undefined`                       | 显式返回 `undefined`                     |
| `never`      | `: never`                           | 永不正常返回：抛异常 / 死循环 / 穷尽检查 |
| `Promise<T>` | `: Promise<number>`                 | 异步函数（配合 `async`）                 |
| 函数类型     | `: () => void`                      | 返回一个函数（高阶函数 / 闭包）          |
| 泛型返回     | `: T` / `: T[]`                     | 返回类型与入参类型关联                   |

```typescript
// void：无返回值
function log(msg: string): void {
    console.log(msg);
}

// never：抛异常，函数不会正常结束
function fail(msg: string): never {
    throw new Error(msg);
}

// 联合类型返回值
function parse(input: string): number | null {
    const n = Number(input);
    return Number.isNaN(n) ? null : n;
}

// 对象返回值
function getPoint(): { x: number; y: number } {
    return { x: 1, y: 2 };
}

// 元组返回值（如 React 的 useState 风格）
function useCounter(): [number, () => void] {
    return [0, () => {}];
}

// 返回函数（高阶函数）
function multiplier(factor: number): (x: number) => number {
    return (x: number) => x * factor;
}
const double = multiplier(2);
double(5); // 10

// 异步函数返回 Promise
async function fetchData(url: string): Promise<string> {
    const res = await fetch(url);
    return res.text();
}

// 泛型返回：返回类型与入参关联
function first<T>(arr: T[]): T | undefined {
    return arr[0];
}
const head = first([1, 2, 3]); // 推断为 number | undefined
```

### 4. 函数定义要点小结

1. **参数与返回值都要显式注解**：公共函数尤其如此，私有小函数可依赖类型推断
2. **参数顺序**：必选参数在前，可选参数（`?`）和默认参数（`=`）在后，剩余参数（`...`）最后
3. **默认参数优于可选参数**：有合理默认值时用 `=`，调用方无需传 `undefined`
4. **参数超过 3 个改用对象解构传参**：配合 `interface` 描述选项，调用更清晰
5. **引用类型参数要防误改**：用 `readonly` / `Readonly<T>` 表达"只读"意图
6. **抛异常的函数返回 `never`**：异步函数显式标注 `Promise<T>`，避免隐式 `Promise<any>`
7. **避免 `any` 参数和返回值**：会失去类型检查意义，不确定时用 `unknown` 收窄

