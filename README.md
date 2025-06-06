# Wallet Mmanagement

## Giới thiệu dự án

Hệ thống quản lý ví điểm thưởng là một ứng dụng cho phép người dùng đăng ký, đăng nhập và quản lý ví điểm thưởng của mình. Hệ thống cung cấp các chức năng chính sau:

### Chức năng chính

1. **Quản lý tài khoản**

   - Đăng ký tài khoản mới
   - Đăng nhập hệ thống
   - Thay đổi mật khẩu
   - Xác thực hai lớp (OTP) khi thay đổi thông tin quan trọng của người dùng

2. **Quản lý ví điểm thưởng**
   - Tạo và quản lý ví điểm thưởng
   - Thực hiện giao dịch chuyển điểm giữa các ví
   - Theo dõi số dư và lịch sử giao dịch
   - Báo cáo hoạt động ví

### Phân quyền người dùng

1. **Người dùng thông thường**

   - Xem và điều chỉnh thông tin cá nhân
   - Quản lý ví điểm thưởng của mình
   - Thực hiện giao dịch chuyển điểm
   - Xem lịch sử giao dịch

2. **Người quản lý**
   - Quản lý thông tin cá nhân
   - Quản lý danh sách người dùng
   - Tạo tài khoản mới
   - Hỗ trợ điều chỉnh thông tin người dùng
   - Theo dõi hoạt động hệ thống

### Tính năng bảo mật

- Mã hóa mật khẩu bằng SHA-256 với salt ngẫu nhiên
- Xác thực hai lớp (OTP) cho các giao dịch quan trọng
- Sao lưu và phục hồi dữ liệu
- Kiểm soát quyền truy cập theo vai trò người dùng

## Hướng dẫn cài đặt và chạy chương trình

### Yêu cầu hệ thống

- C++ compiler hỗ trợ C++11 trở lên (GCC, Clang, hoặc MSVC)
- CMake phiên bản 3.10 trở lên
- Make hoặc Ninja (tùy chọn)
- OpenSSL phiên bản 3.0 trở lên
- libqrencode (thư viện tạo mã QR)

### Cài đặt môi trường

#### Windows

1. Cài đặt MinGW-w64 hoặc Visual Studio với C++ development tools
2. Cài đặt CMake từ [trang chủ CMake](https://cmake.org/download/)
3. Cài đặt MSYS2:
   - Tải MSYS2 từ [trang chủ MSYS2](https://www.msys2.org/)
   - Chạy installer và làm theo hướng dẫn
   - Mở MSYS2 MINGW64 terminal và cập nhật hệ thống:
     ```bash
     pacman -Syu
     ```
   - Cài đặt OpenSSL, qrencode và các công cụ phát triển:
     ```bash
     pacman -S mingw-w64-x86_64-openssl mingw-w64-x86_64-qrencode mingw-w64-x86_64-cmake mingw-w64-x86_64-gcc
     ```
   - Thêm đường dẫn MSYS2 vào biến môi trường PATH (thường là `C:\msys64\mingw64\bin`)

#### macOS

```bash
# Sử dụng Homebrew
brew install cmake openssl qrencode
```

### Biên dịch và chạy

1. Clone repository:

```bash
git clone <repository-url>
cd wallet-management
```

2. Tạo thư mục build và biên dịch:

```bash
# Tạo thư mục build
mkdir build
cd build

# Cấu hình CMake
cmake ..

# Biên dịch
cmake --build .
```

3. Chạy chương trình:

```bash
# Windows
.\wallet_management.exe

# Linux/macOS
./wallet_management
```

### Cấu trúc thư mục

```
wallet-management/
├── CMakeLists.txt
├── main.cpp
├── User.h
├── User.cpp
├── UserManager.h
├── UserManager.cpp
└── data/
    └── users.txt
```

### Lưu ý

- Dữ liệu người dùng được lưu trong thư mục `data/users.txt`
- Thư mục `data` sẽ được tạo tự động khi chạy chương trình lần đầu tiên
- Đảm bảo chương trình có quyền đọc/ghi vào thư mục `data`

## Thành viên tham gia dự án

1. Nguyễn Trung Đức - K24DTCN588
2. Mai Văn Đại - K24DTCN583
3. Nguyễn Thu Trang
