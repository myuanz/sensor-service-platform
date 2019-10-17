FROM python:3.7-alpine
COPY . /app
WORKDIR /app
RUN pip install -i https://pypi.tuna.tsinghua.edu.cn/simple -r req.txt
CMD ["python", "main.py"]
