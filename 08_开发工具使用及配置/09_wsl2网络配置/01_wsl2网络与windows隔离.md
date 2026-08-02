# WSL2 网络与windows侧隔离

现状确认：DNS 现在走的是 dnsTunneling（10.255.255.254，即由 Windows 侧解析），环境里没有代理变量。要做到"WSL 网络完全独立、不随 Windows 抖动"，需要切断 两条依赖链：代理跟随 + DNS 跟随。配置分两侧：

## 一、Windows 侧 `C:\Users\user\.wslconfig`

```shell
[wsl2]
memory=48GB
vmIdleTimeout=86400000

# 显式声明 NAT（独立虚拟网络栈，Clash 系统代理碰不到）
networkingMode=NAT

# 关键：默认值是 true！显式关掉，WSL 才不会跟随 Windows 代理开关
autoProxy=false

# 关键：关掉 DNS 隧道，DNS 不再经由 Windows 解析
dnsTunneling=false
```

注意 autoProxy 和 dnsTunneling 的默认值都是 true，所以必须显式写 false，光删掉是不够的。

## 二、WSL 侧：DNS 改为静态公共 DNS

dnsTunneling=false 后，WSL 默认会把 NAT 网关写进 resolv.conf（仍间接依赖 Windows 解析），所以要彻底独立需固定 DNS：

1. 在 /etc/wsl.conf 追加：

    ```shell
    [network]
    generateResolvConf = false
    ```

2. 把 resolv.conf 从符号链接换成静态文件：

  ```c++
  sudo rm /etc/resolv.conf
  sudo tee /etc/resolv.conf > /dev/null <<'EOF'
  nameserver 223.5.5.5
  nameserver 119.29.29.29
  EOF
  sudo chattr +i /etc/resolv.conf   # 加锁防止被覆盖（可选）
  ```
3. Windows PowerShell 执行 wsl --shutdown，重连后验证：

  ```c++
  cat /etc/resolv.conf      # 应显示 223.5.5.5
  nslookup github.com       # 应能正常解析
  env | grep -i proxy       # 应为空
  ```


  此后 Clash 开关、切节点、换配置都只影响 Windows 自己的应用。唯一的代价：WSL 里 apt/npm/git 不再享受任何代理加速，需要时手动临时指向宿主机（Clash 开 Allow LAN）：

  ```shell
  export hostip=$(ip route show default | awk '{print $3}')
  export https_proxy="http://$hostip:7890" http_proxy="http://$hostip:7890"
  ```

  