# docker使用

参考文档

> [Docker 容器使用 | 菜鸟教程](https://www.runoob.com/docker/docker-container-usage.html)

## docker概述

`Docker`容器是一个轻量级、可移植、自给自足的软件环境，用于运行应用程序。

`Docker`容器将应用程序及其所有依赖项（包括库、配置文件、系统工具等）封装在一个标准化的包中，使得应用能够在任何地方一致地运行。容器是操作系统级别的虚拟化，不需要运行完整的操作系统，启动和运行更为高效。

## docker的组成

`docker`主要由镜像、容器、卷组成

* **镜像**
    * `Docker`镜像（**Docker Image**）是一个只读的模板，用于创建和运行容器。它类似于虚拟机的快照，但更轻量。镜像包含了应用程序及其运行所需的所有依赖项、库、配置文件和环境变量

* **容器**
    * Docker容器是一个轻量级、可执行的独立软件包，它打包了应用程序及其所有的依赖项，以确保应用在不同的计算环境中都能一致运行。容器基于Docker镜像创建，并运行在Docker引擎上。容器是镜像的一个实例，运行时是隔离的、轻量的，且能共享宿主机的操作系统内核

* **卷**

    Docker中的**卷（Volume）**是用于持久化存储和管理数据的机制。它使得容器可以在生命周期之外持久化数据，并且能够在容器之间共享数据。卷是独立于容器生命周期的，因此，即使容器被删除，数据仍然可以保存在卷中

## docker常用命令&参数

**docker常用命令**

| **命令**              | **功能**                                         | **示例**                                   |
| :-------------------- | :----------------------------------------------- | :----------------------------------------- |
| `docker run`          | 启动一个新的容器并运行命令                       | `docker run -d ubuntu`                     |
| `docker ps`           | 列出当前正在运行的容器                           | `docker ps`                                |
| `docker ps -a`        | 列出所有容器（包括已停止的容器）                 | `docker ps -a`                             |
| `docker build`        | 使用 Dockerfile 构建镜像                         | `docker build -t my-image .`               |
| `docker images`       | 列出本地存储的所有镜像                           | `docker images`                            |
| `docker pull`         | 从 Docker 仓库拉取镜像                           | `docker pull ubuntu`                       |
| `docker push`         | 将镜像推送到 Docker 仓库                         | `docker push my-image`                     |
| `docker exec`         | 在运行的容器中执行命令                           | `docker exec -it container_name bash`      |
| `docker stop`         | 停止一个或多个容器                               | `docker stop container_name`               |
| `docker start`        | 启动已停止的容器                                 | `docker start container_name`              |
| `docker restart`      | 重启一个容器                                     | `docker restart container_name`            |
| `docker rm`           | 删除一个或多个容器                               | `docker rm container_name`                 |
| `docker rmi`          | 删除一个或多个镜像                               | `docker rmi my-image`                      |
| `docker logs`         | 查看容器的日志                                   | `docker logs container_name`               |
| `docker inspect`      | 获取容器或镜像的详细信息                         | `docker inspect container_name`            |
| `docker exec -it`     | 进入容器的交互式终端                             | `docker exec -it container_name /bin/bash` |
| `docker network ls`   | 列出所有 Docker 网络                             | `docker network ls`                        |
| `docker volume ls`    | 列出所有 Docker 卷                               | `docker volume ls`                         |
| `docker-compose up`   | 启动多容器应用（从 `docker-compose.yml` 文件）   | `docker-compose up`                        |
| `docker-compose down` | 停止并删除由 `docker-compose` 启动的容器、网络等 | `docker-compose down`                      |
| `docker info`         | 显示 Docker 系统的详细信息                       | `docker info`                              |
| `docker version`      | 显示 Docker 客户端和守护进程的版本信息           | `docker version`                           |
| `docker stats`        | 显示容器的实时资源使用情况                       | `docker stats`                             |
| `docker login`        | 登录 Docker 仓库                                 | `docker login`                             |
| `docker logout`       | 登出 Docker 仓库                                 | `docker logout`                            |

**docker命令常见参数**

| **参数**         | **说明**                                                     | **示例**                                          |
| ---------------- | ------------------------------------------------------------ | ------------------------------------------------- |
| `-d`             | 在后台运行容器（detached mode）。                            | `docker run -d ubuntu`                            |
| `-it`            | 以交互模式运行容器并附加终端（`-i`表示交互模式，`-t`表示分配伪终端）。 | `docker run -it ubuntu /bin/bash`                 |
| `-p`             | 映射主机端口到容器端口。格式：`<host_port>:<container_port>`。 | `docker run -p 8080:80 nginx`                     |
| `--name`         | 为容器指定一个名字。                                         | `docker run --name my_container ubuntu`           |
| `-e`             | 设置环境变量。                                               | `docker run -e "MY_ENV=production" ubuntu`        |
| `-v`             | 挂载主机目录或数据卷到容器。格式：`<host_path>:<container_path>`。 | `docker run -v /host/path:/container/path ubuntu` |
| `--rm`           | 容器停止后自动删除容器。                                     | `docker run --rm ubuntu`                          |
| `--link`         | 连接一个容器到当前容器（已废弃，推荐使用自定义网络）。       | `docker run --link some_db ubuntu`                |
| `-a`             | 显示容器的所有日志（包含stdout、stderr）。                   | `docker logs -a <container_id>`                   |
| `--network`      | 指定容器连接的网络。                                         | `docker run --network my_network ubuntu`          |
| `--restart`      | 设置容器的重启策略。选项包括：`no`（默认），`always`，`unless-stopped`，`on-failure`。 | `docker run --restart=always ubuntu`              |
| `-f`             | 强制执行某些操作，如强制删除容器或镜像。                     | `docker rm -f <container_id>`                     |
| `-l`             | 显示带有标签的输出（用于某些命令，如`docker ps -l`）。       | `docker ps -l`                                    |
| `-q`             | 以简洁模式显示输出，通常用于只需要ID或名称的场景。           | `docker ps -q`                                    |
| `--entrypoint`   | 覆盖容器的默认入口点（`ENTRYPOINT`指令）。                   | `docker run --entrypoint /bin/bash ubuntu`        |
| `--build-arg`    | 在构建镜像时传递构建参数（如`Dockerfile`中的`ARG`）。        | `docker build --build-arg VERSION=1.0 .`          |
| `--no-cache`     | 构建镜像时不使用缓存，强制重新构建所有层。                   | `docker build --no-cache -t my_image .`           |
| `--volumes-from` | 从另一个容器中挂载卷。                                       | `docker run --volumes-from container_name ubuntu` |
| `--cpu-shares`   | 设置容器的CPU优先级。值越大，优先级越高，默认值为1024。      | `docker run --cpu-shares 512 ubuntu`              |
| `--memory`       | 设置容器的内存限制。                                         | `docker run --memory 512m ubuntu`                 |
| `--cpus`         | 限制容器使用的CPU数量。                                      | `docker run --cpus 1.5 ubuntu`                    |
| `--link`         | 连接到其他容器（已废弃）。                                   | `docker run --link container_name`                |
| `--user`         | 指定运行容器的用户。                                         | `docker run --user 1000 ubuntu`                   |
