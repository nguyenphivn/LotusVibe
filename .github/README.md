# fcitx5-lotus — bản fork dùng hằng ngày

Đây là bản fork của [fcitx5-lotus](https://github.com/LotusInputMethod/fcitx5-lotus), bộ gõ tiếng
Việt cho fcitx5, chạy hằng ngày trên CachyOS + KDE Plasma Wayland.

Trang này chỉ ghi **những gì khác với bản gốc**. Hướng dẫn dùng bộ gõ nói chung xem
[README của bản gốc](https://github.com/nguyenphivn/fcitx5-lotus/blob/ban-dung/README.md) hoặc
[trang chủ Lotus](https://lotusinputmethod.github.io/).

- **Nhánh để dùng:** `ban-dung` (nhánh mặc định) = `dev` của bản gốc + 19 miếng vá.
- **Chi tiết từng vá, số đo, tác giả gốc trả lời ra sao, và hướng dẫn cài:**
  [KHAC-GI-SO-VOI-BAN-GOC.md](https://github.com/nguyenphivn/fcitx5-lotus/blob/ban-dung/KHAC-GI-SO-VOI-BAN-GOC.md)

## Khác gì bản gốc

### Sửa lỗi gặp thật

- **Super Smooth hết lặp chữ ở thanh địa chỉ Firefox và Edge** (gõ `tôi` ra `toôi`, kiểu issue
  #190), mà vẫn giữ Super Smooth nguyên bản ở mọi ô khác. Chưa gửi lên bản gốc.
- **Bỏ khoảng chờ vô ích ở app không có surrounding text.** Edge chạy qua XWayland: độ trễ trung vị
  12,8 → 6,9 ms. Bản gốc từ chối (#490).
- **Nhật ký máy chủ ghi ra đĩa từng dòng**, để phân biệt được nhật ký cũ với máy chủ đã chết. Bản
  gốc từ chối (#468).

### Đang chờ tác giả gốc trả lời

- **Chờ sự kiện surrounding text thay vì ngủ theo một hằng số đoán trước** — mặc định TẮT.
- **Chế độ Smooth chờ bằng hẹn giờ** thay vì chặn cả vòng lặp của fcitx5.

Cả hai nằm ở [PR #492](https://github.com/LotusInputMethod/fcitx5-lotus/pull/492).

### Dọn dẹp và hạ tầng

- Bỏ phụ thuộc X11 không dùng tới. Tệp dựng ra giống hệt, máy có X11 vẫn chạy bình thường.
- Siết dịch vụ systemd của máy chủ: `systemd-analyze security` từ 7.0 xuống 2.0.
- Gỡ công tắc `FixUinputWithAck` vốn mặc định tắt.
- Đường dẫn máy chủ lấy từ CMake thay vì viết cứng; thêm biến môi trường `LOTUS_SERVER_PATH`,
  `LOTUS_SOCKET_NAMESPACE` cho máy chủ, `LOTUS_BACKSPACE_GAP_MS`.
- Khoảng cách giữa hai phím xoá mặc định 0 ms thay vì 5 ms. Đây là lựa chọn riêng của bản này.

### Kiểm thử

- 11 bài kiểm thay vì 9: thêm kiểm bất biến trên chuỗi phím ngẫu nhiên, và tái hiện lỗi giữ phím
  của issue #472.

## Bản này KHÔNG sửa

- Chế độ Surrounding Text vẫn lỗi, nhất là trên Firefox.
- Lỗi `eê` ở thanh địa chỉ Chromium là lỗi của Chromium, không vá được từ phía bộ gõ.
- Máy chủ bàn phím ảo chết giữa lúc thay chữ thì bàn phím chết theo.

## Cài

Làm theo mục
[Cài sang máy khác](https://github.com/nguyenphivn/fcitx5-lotus/blob/ban-dung/KHAC-GI-SO-VOI-BAN-GOC.md#cài-sang-máy-khác).
Nhớ gỡ bản Lotus đóng gói sẵn trước, và cài vào `/usr`.
