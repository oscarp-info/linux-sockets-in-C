# comandos linux

```bash
$ netstat -na | grep 8000
tcp        0      0 0.0.0.0:8000            0.0.0.0:*               LISTEN


$ lsof -i -P -n | grep 8000
servidor 11418 devops    3u  IPv4  46284      0t0  TCP *:8000 (LISTEN)

```
