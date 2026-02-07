FROM gcc:latest
WORKDIR /app
COPY . .
RUN g++ -o engine main.cpp
CMD ["./engine"]