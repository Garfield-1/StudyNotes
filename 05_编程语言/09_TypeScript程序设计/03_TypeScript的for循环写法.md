# TypeScript 的 for 循环写法

## 1. 经典 for 循环（C 风格）

三段式：初始化 → 条件判断 → 步进，需要索引或倒序遍历时首选。

```typescript
const arr: number[] = [10, 20, 30];

// 索引变量通常由 0 自动推断为 number，无需显式注解
for (let i = 0; i < arr.length; i++) {
    console.log(i, arr[i]); // 元素类型自动推断为 number
}

// 倒序遍历
for (let i = arr.length - 1; i >= 0; i--) {
    console.log(arr[i]);
}
```

## 2. for...of 循环（遍历值）

遍历可迭代对象的**元素值**，语法最简洁，元素类型自动推断。适用于数组、字符串、Map、Set 等。

```typescript
const arr: number[] = [10, 20, 30];

// 直接拿值
for (const item of arr) {
    console.log(item); // item 自动推断为 number
}

// 需要索引时用 entries()，元组解构类型自动推断
for (const [index, value] of arr.entries()) {
    console.log(index, value); // index: number, value: number
}

// 遍历字符串
for (const ch of "hello") {
    console.log(ch); // ch: string
}

// 遍历 Map / Set
const map = new Map<string, number>([["a", 1], ["b", 2]]);
for (const [key, value] of map) {
    console.log(key, value); // key: string, value: number
}
```

## 3. for...in 循环（遍历键）

遍历对象的**可枚举属性键**（含继承的非 symbol 属性），键的类型是 `string`。主要用于遍历普通对象，**不建议用来遍历数组**。

```typescript
const user = { name: "张三", age: 25 };

for (const key in user) {
    // key: string，访问属性需断言或类型收窄
    console.log(key, user[key as keyof typeof user]);
}

// 推荐写法：先用 keyof 约束，获得精确的键类型
for (const key of Object.keys(user) as (keyof typeof user)[]) {
    console.log(key, user[key]);
}

// ⚠️ 数组用 for...in 得到的是字符串键 "0"、"1"，还可能遍历到继承属性，不推荐
const nums: number[] = [10, 20];
for (const index in nums) {
    console.log(index, nums[index]); // index 是 string 类型
}
```

## 4. while / do...while 循环

条件驱动的循环；`do...while` 至少执行一次。

```typescript
let count = 0;

while (count < 3) {
    console.log(count);
    count++;
}

do {
    console.log(count); // 先执行一次，再判断条件
    count++;
} while (count < 5);
```

## 5. 数组方法 forEach（非循环语句，但常用于替代循环）

数组的高阶方法，无法中途 `break` / `continue`（需提前退出时应改用 `for...of` 或 `some` / `every`）。

```typescript
const arr: number[] = [10, 20, 30];

arr.forEach((value, index) => {
    console.log(index, value); // value: number 自动推断
});
```

## 6. for await...of 循环（异步迭代）

遍历异步可迭代对象（如异步生成器），需在 `async` 函数中使用。

```typescript
async function* asyncNumbers(): AsyncGenerator<number> {
    yield 1;
    yield 2;
}

async function main(): Promise<void> {
    for await (const n of asyncNumbers()) {
        console.log(n); // n: number，逐个 await 后处理
    }
}
```

## 7. 循环写法对比与选择

| 写法                   | 遍历对象                  | 拿到内容                      | 支持 break/continue | 适用场景                       |
| ---------------------- | ------------------------- | ----------------------------- | ------------------- | ------------------------------ |
| `for`                  | 数组                      | 索引 + 值                     | ✅                   | 需索引、倒序、复杂步进控制     |
| `for...of`             | 数组 / 字符串 / Map / Set | 值（可配 `entries()` 拿索引） | ✅                   | **遍历集合的首选**             |
| `for...in`             | 普通对象                  | 属性键（string）              | ✅                   | 遍历对象自有属性，避免用于数组 |
| `while` / `do...while` | 任意条件                  | —                             | ✅                   | 循环次数不确定                 |
| `forEach`              | 数组                      | 值 + 索引                     | ❌                   | 简单全量遍历，不可提前退出     |
| `for await...of`       | 异步可迭代对象            | 逐个 await 的值               | ✅                   | 异步流 / 异步生成器            |

## 8. 要点小结

1. **遍历数组优先用 `for...of`**：简洁且类型自动推断，需要索引时改用经典 `for`
2. **不要用 `for...in` 遍历数组**：键是字符串且会包含继承属性；对象遍历建议配合 `keyof` 收窄类型
3. **循环内 `break` 提前退出、`continue` 跳过本次**：`forEach` 不支持，提前退出场景请用 `for...of`
4. **异步场景用 `for await...of`**：需配合 `async` 函数与 `AsyncIterable` 数据源
5. **循环变量用 `const`（`for...of`）或 `let`**：避免 `var` 带来的闭包陷阱