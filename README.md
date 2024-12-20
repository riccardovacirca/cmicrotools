# cmicrotools
C/C++ Micorservice Development Tools

## Install

Clone the repository to a folder with the name of the service

```bash
git clone https://github.com/riccardovacirca/cmicrotools.git hello-service
cd hello-service && mv .env-example .env
```

Edit <code>.env</code>

```bash
REPO_NAME="hello-service"
```

Add github.com credentials to <code>.env</code> to create a repository:

```bash
USERNAME="your-github-username"
TOKEN="your-github-token"
PRIVATE="private"
```
