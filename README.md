# InMemoryDB Server  

A custom **in-memory database server** built in **C++**, inspired by **Redis**.  
It implements the Redis Serialization Protocol (RESP), socket-based communication, persistence storage, and is fully compatible with the official **Redis CLI**.  

---

## 🚀 Features  

- **RESP Protocol Parser** – Fully supports Redis Serialization Protocol.  
- **In-Memory Data Store** – Lightweight key-value database implementation.  
- **Persistence** – Store data on disk and reload across sessions.  
- **Socket-Based Server** – Handles multiple client connections.  
- **Redis CLI Compatible** – Works seamlessly with the official `redis-cli`.  
- **Core Redis Commands** – `SET`, `GET`, `DEL`, `EXISTS`, and more list and hash commands.  

---

## 📦 Installation  

### Prerequisites  
- **GNU Make**  
- **g++** (or another modern C++ compiler)  
- **Redis CLI** (install via package manager: `sudo apt-get install redis-tools` or `brew install redis`)  

### Clone & Build  
```bash
git clone https://github.com/Satyam1923/inMemoryDB-server.git
cd inMemoryDb-Server
make
```

This will build the `my_redis_server` binary.  

---

## ⚡ Usage  

Start the server (default port: **6379**):  
```bash
./my_redis_server
```

Open another terminal and connect using the official **Redis CLI**:  
```bash
redis-cli 
```

### Example Session  
```bash
127.0.0.1:6379> SET name "Satyam"
OK
127.0.0.1:6379> GET name
"Satyam"
127.0.0.1:6379> DEL name
(integer) 1
127.0.0.1:6379> EXISTS name
(integer) 0
```

---

## 🛠️ Project Structure  

```
├── src/                  #contains server database and command handlder class
├── include/                # all classes defined here
├── Makefile             # Build configuration
├── README.md
└── ...
```

---



## 🤝 Contributing  

Contributions, issues, and feature requests are welcome!  
Feel free to open a PR or create an issue.  

---

## 📜 License  

This project is licensed under the **MIT License** – see the [LICENSE](LICENSE) file for details.  
