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
   - Xem và điều chỉnh thông tin cá nhân
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

4. Đăng nhập vào chương trình dùng tài khoản admin mặc định:
   - username: admin
   - password: password

### Xử lý lỗi

Nếu sau khi cài đặt môi trường mà vẫn không build được, hãy kiểm tra và điều chỉnh các đường dẫn trong file `CMakeLists.txt`:

#### Windows

1. Kiểm tra đường dẫn MSYS2:

   - Mở file `CMakeLists.txt`
   - Tìm và kiểm tra các biến đường dẫn:
     ```cmake
     set(OPENSSL_ROOT_DIR "C:/msys64/mingw64")
     set(QRENCODE_ROOT_DIR "C:/msys64/mingw64")
     ```
   - Đảm bảo đường dẫn này khớp với nơi bạn cài đặt MSYS2
   - Nếu bạn cài MSYS2 ở ổ đĩa khác, hãy thay đổi `C:` thành ổ đĩa tương ứng

2. Kiểm tra tên file thư viện:

   - Kiểm tra xem các file thư viện có tồn tại không:
     ```bash
     # Trong MSYS2 terminal
     ls C:/msys64/mingw64/lib/libssl*
     ls C:/msys64/mingw64/lib/libcrypto*
     ls C:/msys64/mingw64/lib/libqrencode*
     ```
   - Nếu tên file khác với trong CMakeLists.txt, hãy cập nhật lại

3. Kiểm tra file DLL:
   - Kiểm tra xem các file DLL có tồn tại không:
     ```bash
     # Trong MSYS2 terminal
     ls C:/msys64/mingw64/bin/libssl*
     ls C:/msys64/mingw64/bin/libcrypto*
     ls C:/msys64/mingw64/bin/libqrencode*
     ```
   - Nếu tên file DLL khác, hãy cập nhật lại trong phần copy DLL của CMakeLists.txt

#### macOS

1. Kiểm tra cài đặt Homebrew:

   ```bash
   # Kiểm tra phiên bản Homebrew
   brew --version

   # Cập nhật Homebrew nếu cần
   brew update
   ```

2. Kiểm tra các thư viện đã cài đặt:

   ```bash
   # Kiểm tra OpenSSL
   brew list openssl
   # Kiểm tra qrencode
   brew list qrencode
   ```

3. Kiểm tra đường dẫn trong CMakeLists.txt:

   - Mở file `CMakeLists.txt`
   - Tìm và kiểm tra các biến đường dẫn:
     ```cmake
     set(OPENSSL_ROOT_DIR "/opt/homebrew/opt/openssl@3")
     ```
   - Nếu đường dẫn khác, hãy cập nhật lại cho phù hợp với vị trí cài đặt của bạn
   - Kiểm tra đường dẫn thực tế:
     ```bash
     brew --prefix openssl
     brew --prefix qrencode
     ```

4. Kiểm tra quyền truy cập:

   ```bash
   # Kiểm tra quyền truy cập thư mục build
   ls -la build/

   # Nếu cần, cấp quyền truy cập
   chmod -R 755 build/
   ```

5. Kiểm tra phiên bản CMake:

   ```bash
   cmake --version
   # Đảm bảo phiên bản >= 3.10
   ```

6. Sau khi sửa, xóa thư mục build và build lại:
   ```bash
   rm -rf build
   mkdir build
   cd build
   cmake ..
   cmake --build .
   ```

### Cấu trúc thư mục

```
wallet-management/
├── CMakeLists.txt              # File cấu hình CMake
├── README.md                   # Tài liệu hướng dẫn
├── .gitignore                 # File cấu hình Git ignore
├── main.cpp                   # File chính của chương trình
├── User.h                     # Header file cho class User
├── User.cpp                   # Implementation của class User
├── UserManager.h             # Header file cho class UserManager
├── UserManager.cpp           # Implementation của class UserManager
├── Wallet.h                  # Header file cho class Wallet
├── Wallet.cpp                # Implementation của class Wallet
├── DataManager.h            # Header file cho class DataManager
├── DataManager.cpp          # Implementation của class DataManager
├── TransactionManager.h     # Header file cho class TransactionManager
├── TransactionManager.cpp   # Implementation của class TransactionManager
├── OTPManager.h            # Header file cho class OTPManager
├── OTPManager.cpp          # Implementation của class OTPManager
├── PasswordHasher.h        # Header file cho class PasswordHasher
├── PasswordHasher.cpp      # Implementation của class PasswordHasher
├── QRPrinter.h            # Header file cho class QRPrinter
├── QRPrinter.cpp          # Implementation của class QRPrinter
├── lib/                   # Thư mục chứa các thư viện bên thứ ba
│   └── cotp/             # Thư viện COTP cho xác thực hai lớp
├── data/                 # Thư mục lưu trữ dữ liệu
│   ├── users.txt        # File lưu thông tin người dùng
│   ├── wallets.txt      # File lưu thông tin ví
│   └── transactions.txt # File lưu lịch sử giao dịch
└── build/               # Thư mục chứa các file build
```

### Lưu ý

- Dữ liệu được lưu trong thư mục `data/`:
  - `users.txt`: Lưu thông tin người dùng
  - `wallets.txt`: Lưu thông tin ví
  - `transactions.txt`: Lưu lịch sử giao dịch
- Thư mục `data` sẽ được tạo tự động khi chạy chương trình lần đầu tiên
- Đảm bảo chương trình có quyền đọc/ghi vào thư mục `data`

## Thành viên tham gia dự án

1. Nguyễn Trung Đức - K24DTCN588
2. Mai Văn Đại - K24DTCN583
3. Nguyễn Thu Trang - K24DTCN645
