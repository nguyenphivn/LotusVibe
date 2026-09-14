# Bản này khác gì so với Lotus gốc

Đây là bản fork của [fcitx5-lotus](https://github.com/LotusInputMethod/fcitx5-lotus), dùng hằng
ngày trên máy CachyOS + KDE Plasma Wayland. Tệp này ghi lại **từng miếng vá**: vá gì, vì sao, đã
gửi ngược lên chưa, và tác giả trả lời ra sao.

## Lấy bản nào

**Nhánh `ban-dung`.** Đó là nhánh duy nhất nên lấy để dùng hoặc để thử trên máy khác.

```
git clone https://github.com/nguyenphivn/fcitx5-lotus.git
cd fcitx5-lotus
git checkout ban-dung
```

`ban-dung` = `upstream/dev` + đúng **20 miếng vá**, không thiếu commit nào của tác giả. Từ
13/09/2026 đây cũng là **nhánh mặc định** của fork.

Mấy nhánh khác là nhánh làm việc, **đừng lấy**:

| Nhánh | Là gì | Có nên lấy |
| --- | --- | --- |
| `ban-dung` | Bản gom gọn, đang dùng hằng ngày | **Có** |
| `pr/*`, `fix/*`, `do/*`, `proto/*` | Từng nhánh nhỏ để gửi PR hoặc để đo | Không |
| `main`, `dev` | Chép theo kho gốc | Không |

Các nhánh gom cũ `tong-hop`, `tong-hop-v2` và `thu/bo-fixack` **đã xoá ngày 13/09/2026**. Mọi vá
còn giá trị của chúng đều nằm trong `ban-dung`.

## Cài sang máy khác

Các bước dưới đây cho Arch và CachyOS. Distro khác thì gói cần cài lấy ở mục "Yêu cầu hệ thống"
trong `README.md`, các bước còn lại giống hệt.

**1. Gỡ bản Lotus đóng gói sẵn, nếu máy đã có.** Không gỡ thì tệp của hai bản đè lên nhau, và lần
cập nhật hệ thống sau sẽ báo lỗi tệp xung đột.

```
pacman -Qs fcitx5-lotus
sudo pacman -R fcitx5-lotus
```

**2. Cài công cụ dựng.**

```
sudo pacman -S --needed cmake extra-cmake-modules gcc go git python make pkgconf acl fcitx5 libinput hicolor-icon-theme python-qtpy python-dbus librsvg
```

**3. Tải mã và dựng.** Bắt buộc cài vào `/usr`: dịch vụ nền ghi cứng đường
`/usr/bin/fcitx5-lotus-server`, cài chỗ khác thì máy chủ bàn phím ảo không chạy. Nhớ
`--recurse-submodules`, vì lõi bộ gõ nằm ở kho con `bamboo-core`.

```
git clone --recurse-submodules -b ban-dung https://github.com/nguyenphivn/fcitx5-lotus.git
cd fcitx5-lotus
cmake -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_INSTALL_LIBDIR=/usr/lib
cmake --build build -j8
sudo cmake --install build
```

`CMAKE_BUILD_TYPE=Release` là để bật tối ưu tốc độ. Bỏ cờ này thì bản dựng không được tối ưu.

**4. Bật máy chủ bàn phím ảo cho tài khoản của mình.**

Bốn lệnh đầu tạo người dùng hệ thống `uinput_proxy`, nạp mô-đun bàn phím ảo và nạp luật quyền truy
cập vừa cài, để khỏi phải khởi động lại máy.

```
sudo systemd-sysusers
sudo modprobe uinput
sudo udevadm control --reload-rules
sudo udevadm trigger --subsystem-match=misc --subsystem-match=input
sudo systemctl daemon-reload
sudo systemctl enable --now fcitx5-lotus-server@$(whoami).service
systemctl status fcitx5-lotus-server@$(whoami).service       # phải thấy active (running)
```

**5. Thêm bộ gõ.** Khởi động lại fcitx5 (hoặc đăng xuất rồi vào lại), mở "Fcitx5 Configuration" và
thêm Lotus. Trên KDE Wayland: System Settings → Virtual Keyboard → chọn "Fcitx 5".

**6. Đặt luật theo app giống máy gốc.** Sửa `~/.config/fcitx5/conf/lotus-app-rules.conf` (hoặc đặt
trong cửa sổ cài đặt Lotus), rồi khởi động lại fcitx5:

```
firefox=3
microsoft-edge=3
Alacritty=3
```

Luật `firefox=3` chỉ an toàn trên bản này, vì cần vá lá chắn thanh địa chỉ ở nhóm E. Lá chắn nhận
Firefox theo tên chương trình: bản Firefox mang tên khác (LibreWolf, Firefox Developer Edition)
**chưa** được nhận. Trình duyệt họ Chromium khác (Chromium, Brave) tự khai ô địa chỉ giống Edge nên
nhiều khả năng chạy, nhưng **chưa đo**.

Máy gốc còn bật `WaitSurroundingEvent=True` trong `~/.config/fcitx5/conf/lotus.conf` (vá nhóm B,
mặc định tắt). Vá thanh địa chỉ không phụ thuộc tuỳ chọn này, nhưng mọi lượt đo trên máy gốc đều
chạy khi nó bật.

**Cập nhật bản mới về sau:** trong thư mục `fcitx5-lotus`, chạy `git pull --recurse-submodules`,
lặp lại bước 3, rồi `sudo systemctl restart fcitx5-lotus-server@$(whoami).service` và khởi động lại
fcitx5.

## Dựng và chạy bộ kiểm

Phần kiểm thử mặc định TẮT trong CMake, phải bật bằng `-DBUILD_TESTING=ON`, không thì `ctest` báo 0
bài:

```
cmake -B build-test -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=ON
cmake --build build-test -j8
unshare -Urn ctest --test-dir build-test        # phải ra 12/12
```

Bản gốc `dev` chạy 9 bài. Bản này 12: thêm hai bài kiểm ở nhóm D, và một bài đi kèm vá icon ở
nhóm E.

## Hiệu năng so với bản gốc (chế độ uinput)

Bảng này cũng có ở trang đầu `.github/README.md`; sửa số thì sửa cả hai. Mã trong ngoặc là mục
trong `RADAR.md` của repo workbench, nơi ghi cách đo.

| Đo cái gì | Bản gốc → bản này | Điều kiện |
| --- | --- | --- |
| Thay một từ, từ lúc gửi phím xoá tới lúc gõ chữ mới: xoá 2 / 3 / 4 chữ | 14,5 / 21,8 / 31,2 → 12,4 / 16,3 / 20,5 ms (trung bình hai lượt) | Konsole, Smooth, 8/8 mỗi bản, bản gốc chạy cạnh bản mình cùng khung đo, 12/09/2026. Bản mình lúc đó chưa gom: khoảng cách phím xoá 2 ms, còn vá #489. Chỉ số này không tính thời gian bên trong máy chủ bàn phím ảo (B43ac) |
| Khoảng cách giữa hai phím xoá, trung vị | 5,2 → 0,08–0,10 ms | Konsole, ô soạn Edge, Firefox hồ sơ riêng, Edge qua XWayland, 8/8; mức 2 ms là 2,18–2,21, mức 1 ms là 1,18–1,21 (B43ab, B43af, B43ai, B43aj) |
| Chờ vô ích ở app không khai surrounding text, trung vị | 12,8 → 6,9 ms (xoá 4 chữ: 21,2 → 14,9 ms) | Edge qua XWayland, 16/16 lượt vào đúng nhánh vá; app Wayland thuần 0/16 nên không đổi (B43ae bị B43ag lật) |

**Không cộng dồn các dòng trên:** mỗi dòng đo một thay đổi, ở bản dựng và app khác nhau. **Chưa có
phép đo trọn vẹn** so nhánh `ban-dung` hiện tại (khoảng cách 0 ms) với bản gốc.

**Lời khai kèm:** từng lần chênh vài mili giây, dưới ngưỡng cảm nhận. Mức 0 ms là lựa chọn riêng của
máy này (đề xuất cho bản gốc là 2 ms); bàn phím ảo trong bộ đo gõ khoảng 25 ms một phím nên chưa bao
giờ chạm trường hợp phím xoá chen vào lúc người gõ nhanh. Thay `sleep_for` bằng hẹn giờ ở chế độ
Smooth (`edcc370`) chưa đo riêng; số nền của bản gốc là 4% phím làm fcitx5 đứng ≥ 8 ms, lâu nhất
22,2 ms, trên 428 phím (B33). Super Smooth và Smooth chưa từng đo khác nhau về thời gian (B43v);
"Super Smooth gõ tốt hơn" là chủ máy gõ tay.

## Nhóm A — lỗi gặp thật, đã báo, tác giả từ chối vá

Hai miếng này tác giả đã đóng issue mà không sửa mã, và cả hai vẫn đứng vững. Mục thứ ba bên
dưới là một miếng vá **đã bị rút lại** vì mình báo sai — giữ lại để khỏi ai làm lại.

### ĐÃ RÚT LẠI — `f9ccb50` máy chủ đừng tự bật chạm-để-bấm (issue #494)

**Miếng vá này đã bị gỡ khỏi `ban-dung`.** Giữ mục này lại làm bài học, đừng làm lại.

Mình báo issue #494 với tiền đề "máy mình tắt chạm-để-bấm", rồi viết thêm một bình luận phản biện
cũng dựa trên tiền đề đó. **Tiền đề đó SAI.** Chủ máy chưa bao giờ tắt chạm-để-bấm; mình tự suy
ra mà không hỏi. Tác giả upstream đúng ngay từ phản hồi đầu tiên.

Đo lại bằng hai dụng cụ, mỗi cái nhìn một ngữ cảnh:

| Nơi | chạm-để-bấm | tắt-khi-đang-gõ |
| --- | --- | --- |
| KWin, tức desktop thật (`busctl` hỏi `org.kde.KWin`) | BẬT, và mặc định cũng BẬT | BẬT |
| Ngữ cảnh libinput riêng dựng y như máy chủ, không ép gì | TẮT | BẬT |

Nên hai dòng ép bật tap trong máy chủ đang làm ngữ cảnh riêng **KHỚP** với desktop, chứ không
phải ghi đè lên nó. Gỡ chúng đi mới là làm Lotus lệch khỏi phần còn lại của máy: người dùng chạm
để dời con trỏ, hệ thống bấm thật, mà Lotus không reset từ đang gõ dở.

**Sai ở đâu, cụ thể:** số đo "16 cú bấm xuống còn 2" là đúng, nhưng mình rút ra kết luận sai từ
nó. Nó chỉ chứng minh **mặc định của THIẾT BỊ** là tắt tap. Nó không nói gì về **cài đặt hiệu lực
của người dùng**, thứ nằm trong ngữ cảnh riêng của compositor và không có cách nào suy ra từ ngữ
cảnh khác. Mình còn suy thêm một tầng nữa cũng sai: thấy `kcminputrc` không có khoá `TapToClick`
rồi kết luận là tắt, trong khi không có khoá nghĩa là **theo mặc định của KDE**, mà mặc định đó
là BẬT.

**Bài học, áp cho mọi lần sau:** cài đặt hiệu lực của người dùng thì **hỏi chủ máy hoặc hỏi
compositor**, đừng suy từ tệp cấu hình và đừng suy từ mặc định của thiết bị. Câu lệnh hỏi thẳng:

```
busctl --user get-property org.kde.KWin \
  /org/kde/KWin/InputDevice/eventN org.kde.KWin.InputDevice tapToClick
```

**Triệu chứng gốc vẫn chưa có lời giải chắc chắn.** Chủ máy báo gõ `Nguyễn Trãi` trong Lark ra
`Nguyễn Traix`, chập chờn. Giả thuyết còn lại mạnh nhất: Lark chạy trong Firefox, mà lúc đó luật
đặt `firefox=3` tức Super Smooth — chế độ uinput duy nhất bị tắt lá chắn chống nhân đôi chữ. Nếu
vậy thì touchpad vô can từ đầu. **Chưa kiểm chứng.** Lưu ý: từ vá lá chắn thanh địa chỉ ở nhóm E,
Lark vẫn chạy Super Smooth KHÔNG có lá chắn (bộ lọc cố ý loại ô Lark), nên nếu `Traix` quay lại
thì giả thuyết này đứng, còn nếu không quay lại thì nó yếu đi.

### `770f02e` — bỏ chờ retry vô ích ở app không có surrounding text

**Nguyên nhân:** đường uinput chờ thêm 6 ms để app gửi lại surrounding text, ngay cả với app đã
tự khai là không có. Cái chờ đó không thể thành công.

**Đo:** trên Edge chạy qua XWayland, độ trễ trung vị 12,8 → 6,9 ms, 16/16 lượt đi vào đúng
nhánh được vá.

**Upstream:** [issue #490](https://github.com/LotusInputMethod/fcitx5-lotus/issues/490) — đóng
kiểu NOT_PLANNED.

**Ghi chú quan trọng:** lần đo ĐẦU kết luận sai là "không đổi gì", vì bộ đích chỉ có app Wayland
thuần. App Wayland luôn khai có surrounding text nên không bao giờ đi vào nhánh này. Phải đo
trên app qua XWayland mới thấy. Đừng lặp lại lỗi đó.

### `9c82072` — nhật ký xả đĩa mỗi dòng, không chỉ khi có cảnh báo

**Nguyên nhân:** bản gốc chỉ xả đĩa khi mức log từ WARN trở lên. Mọi dòng quan trọng lúc khởi
động đều là mức INFO, nên nhật ký đứng yên trong khi máy chủ vẫn chạy bình thường. Người đọc
không phân biệt được nhật ký cũ với máy chủ đã chết.

**Giá phải trả:** vài dòng mỗi phút, không đo được.

**Upstream:** [issue #468](https://github.com/LotusInputMethod/fcitx5-lotus/issues/468) — đóng
kiểu NOT_PLANNED.

## Nhóm B — đang chờ tác giả trả lời

Hai commit, gửi chung ở [PR #492](https://github.com/LotusInputMethod/fcitx5-lotus/pull/492)
(bản nháp). Liên quan hai issue còn mở, #487 và #488.

- **`9252326`** — chờ SỰ KIỆN surrounding text thay vì ngủ theo một hằng số đoán trước.
  **Mặc định TẮT**, phải bật trong cấu hình mới có tác dụng.
- **`edcc370`** — ở nhánh dự phòng của chế độ Smooth, chờ app bằng hẹn giờ thay vì `sleep_for`.
  `sleep_for` chặn vòng lặp sự kiện của cả fcitx5, không riêng Lotus.

**Khe hở đã khai thẳng trong PR, đừng rút lại:** nếu người dùng đổi cửa sổ trong vòng 50 ms thì
ô cũ có thể mất chữ. Cửa sổ quá hẹp nên thực tế không gặp.

## Nhóm C — dọn dẹp và hạ tầng, chưa gửi upstream

- **`48e3122` bỏ phụ thuộc X11 không dùng.** Không có dòng mã nào trong `src/`, `server/`,
  `test/` gọi X11. Đo `ldd`: 0 thư viện X11 cả trước lẫn sau, tệp sinh ra giống hệt. Cái đổi là
  máy Wayland thuần không còn phải cài gói phát triển X11 để dựng thứ chẳng đụng tới X11.
  Đối chứng dương cho phép đo: `ldd` trên mô-đun xcb của fcitx5 ra 8 thư viện.
- **`a87bc7b` siết cứng dịch vụ systemd.** `systemd-analyze security` từ 7.0 MEDIUM xuống 2.0
  OK, và đã cài chạy thật để chắc dịch vụ không hỏng. Hai chỉ thị cố ý KHÔNG bật vì cả hai đều
  làm hỏng dịch vụ: `PrivateNetwork` (udev gửi sự kiện qua netlink, netlink theo từng không gian
  mạng) và `ProtectProc` (cổng xác thực phải đọc `/proc/<pid>/exe` của tiến trình thuộc người
  dùng khác). Lưu ý `PrivateTmp` làm nhật ký chuyển vào `/tmp/systemd-private-*/`, đọc phải có
  quyền root và đường cũ ngừng cập nhật.
- **`0355cfb` gỡ `FixUinputWithAck`, cờ Chromium và tệp `src/ack-apps.h`.** Công tắc này vốn mặc
  định TẮT nên gỡ đi hành vi không đổi. Đã soi cả 5 chỗ dùng để chắc mỗi chỗ đặc biệt hoá đúng
  cho nhánh tắt.
- **`86ea551` lấy đường dẫn máy chủ từ CMake thay vì viết cứng `/usr/bin`.**
  ⚠️ **Gửi lẻ commit này lên upstream sẽ làm gói Nix GÃY**, vì Nix dùng
  `substituteInPlace --replace-fail` trên đúng chuỗi literal đó. Phải gửi kèm bản sửa tệp Nix
  trong cùng một PR.
- **`220b57c` biến môi trường `LOTUS_SERVER_PATH`** để chỉ định máy chủ mong đợi. Thiếu biến này
  thì mô-đun từ chối socket chuột, và tính năng bấm chuột ngắt từ chết âm thầm.
- **`6925601` máy chủ hiểu `LOTUS_SOCKET_NAMESPACE`** giống mô-đun. Nhờ vậy chạy được một cặp
  mô-đun + máy chủ riêng bên cạnh bản đóng gói sẵn, không giẫm chân nhau.
- **`f14c0da` núm vặn `LOTUS_BACKSPACE_GAP_MS`** để đo nhịp gửi phím xoá.
- **`d2a46e6` khoảng cách phím xoá mặc định 0 ms thay vì 5.** Đo `khoang_cach_xoa.py macdinh`:
  0,10 ms, 8/8 trên bốn đích. **Đây là lựa chọn riêng của máy này, không phải đề xuất cho
  upstream** — mức đề xuất cho upstream là 2 ms, vì mức 0 bỏ hẳn yêu cầu khe im lặng. Dấu hiệu
  DUY NHẤT để quay lại mức 2 là **sót chữ hoặc thừa chữ khi gõ nhanh**, không phải cảm giác
  nhanh chậm: chênh 0 với 2 chỉ khoảng 6 ms mỗi lần xoá 4 chữ, dưới ngưỡng cảm nhận.

## Nhóm D — bộ kiểm thêm vào

Sáu commit. Hai bài kiểm mới ở đây, cộng bài kiểm của vá icon ở nhóm E, là lý do bản này chạy 12
bài thay vì 9.

- **`c612050`** bài kiểm bất biến trên chuỗi phím ngẫu nhiên.
- **`b789d26`** đối chứng dương cho các bất biến P2 đến P5, tức chứng minh bài kiểm THẤY được
  lỗi khi lỗi có mặt, chứ không phải xanh vì nó không kiểm gì.
- **`a4c9759`** tái hiện vòng lặp giữ phím của issue #472.
- **`971c5fd`** mô hình đúng phím thô mà cửa sổ nhận được ở chế độ Smooth.
- **`a8b4b09`** tách socket riêng cho `smooth_buffered_key_replay` để hai bài kiểm không giẫm
  chân nhau.
- **`d5a6ab1`** hoà giải các nhánh đã gộp với bộ khung kiểm hiện tại của `dev`.

## Nhóm E — sửa lỗi gặp thật, chưa gửi upstream

### Super Smooth có lá chắn chống lặp chữ, nhưng CHỈ ở thanh địa chỉ trình duyệt

**Triệu chứng:** ở chế độ Super Smooth, gõ tiếng Việt vào thanh địa chỉ bị lặp chữ đầu đúng kiểu
issue #190 (gõ `tôi` ra `toôi`), trên cả Firefox lẫn Edge. Chế độ Smooth không lặp, nhưng chủ máy
gõ thử thấy Super Smooth tốt hơn ở gần như mọi chỗ khác.

**Nguyên nhân, trong mã gốc:** hai chế độ chỉ khác nhau đúng một chỗ. `performReplacement` bọc lá
chắn `isAutofillCertain` bằng `realMode != LotusMode::SuperSmooth`, tức Super Smooth bỏ lá chắn ở
mọi ô.

**Vá:** Super Smooth vẫn bỏ lá chắn, trừ khi ô đang gõ là thanh địa chỉ trình duyệt:

- **Edge/Chromium** tự khai `CapabilityFlag::Url` cho thanh địa chỉ, và báo đúng phần tự điền
  đang bôi đen.
- **Firefox** không khai cờ đó, và báo cho bộ gõ là KHÔNG bôi đen dù trên màn hình có tô. Nhận ra
  theo hình dạng: sau con trỏ có chữ, không xuống dòng, và toàn ký tự của địa chỉ web. Ở đó lá chắn
  bắn khi phép đoán gốc đồng ý, **hoặc** khi phần trước con trỏ đúng bằng chữ đang gõ (chữ đầu tiên
  của ô). Nhánh thứ hai cần vì phép đoán gốc so với một độ dài tự đếm, mà độ dài đó giữ số của lần
  gõ trước sau khi xoá trắng ô. Phím xoá thừa ở đầu ô không xoá gì nên đoán sai cũng vô hại.

**Đo 13/09/2026** bằng dòng ghi tạm, chỉ số đếm, không lưu chữ, đã gỡ khỏi bản dùng:

| Ô | Lá chắn được xét | Kết quả chủ máy gõ |
| --- | --- | --- |
| Thanh địa chỉ Edge | có, nhờ cờ Url; 6/6 lần có gợi ý thì bắn | hết lặp |
| Thanh địa chỉ Firefox | có, nhờ hình dạng đuôi; 2/2 lần có gợi ý thì bắn | hết lặp |
| Lark trong Firefox, ô web Edge, Alacritty | không, Super Smooth nguyên bản | ổn |

**Bẫy đã gặp, đừng nới bộ lọc:** bản đầu chỉ cấm khoảng trắng sau con trỏ. Lark luôn giữ 3 ký tự
vô hình sau con trỏ nên lọt qua, và đã bắn lá chắn nhầm một lần trong Lark. Bộ lọc theo tập ký tự
địa chỉ web loại cả 19/19 lần thay chữ ở Lark.

**Bài học đo:** bảng trên lúc đầu chỉ có 2 lần Firefox có gợi ý. Dùng thật thì còn lặp khoảng 27%
(bộ đo tự đọc lại ô sau mỗi lần sửa dấu: 6/22 lần lặp, 5 trong số đó do độ dài tự đếm cũ). Sau khi
thêm nhánh "chữ đầu tiên của ô": 7/7 lần có gợi ý ra đúng, 0 lặp, 0 mất chữ, chủ máy gõ thật báo
hết lặp. Mẫu vẫn nhỏ.

**Còn sót có thể:** nếu Firefox chưa kịp báo phần tự điền lúc bộ gõ quyết định thì không nhánh nào
nhận ra. Lượt đo đầu gặp 1/22 lần, lượt sau không gặp.

**Cần luật:** `firefox=3`, `microsoft-edge=3` (xem mục cấu hình bên dưới).

**Lỗi lặp chữ đầu ở thanh địa chỉ Chromium (`eê`, `toôi`) với chế độ uinput: đã sửa triệt để.**
Chromium báo đúng phần tự điền đang bôi đen, nên ở Edge lá chắn đi theo cơ chế chắc chắn, không phải
phép đoán. Chủ máy xác nhận ngày 13/09/2026. Chế độ Surrounding Text **không** được sửa: ở chế độ đó
lỗi này vẫn là lỗi của Chromium (đã báo Chromium số 557316480), xem mục "không sửa".

### Icon chữ V màu đen trên panel tối của KDE (issue #374)

**Triệu chứng:** trên KDE Plasma, màu icon để `Auto`, chữ V ở khay hệ thống màu đen nằm trên panel
màu đen, gần như không thấy. Gặp ngay với giao diện mặc định của Fedora 44.

**Nguyên nhân, trong mã gốc:** `isDarkMode()` hỏi portal (`org.freedesktop.appearance
color-scheme`). Trên KDE, portal trả lời theo màu cửa sổ ứng dụng (`kdeglobals`), còn panel do
Plasma Style (`plasmarc`) tô. Giao diện mặc định Fedora đặt `ColorScheme=BreezeLight` nhưng Plasma
Style là `breeze-dark`, ghi ở `~/.config/kdedefaults`. Portal trả 2 (sáng) nên Lotus chọn icon đen.

**Vá:** tệp mới `src/lotus-plasma-theme.cpp`. Khi `XDG_CURRENT_DESKTOP` có `KDE`, Lotus đọc thẳng
tệp cấu hình, không gọi tiến trình con:

- Lấy tên Plasma Style theo thứ tự đè của KConfig: thư mục người dùng, rồi `kdedefaults`, rồi
  `/etc/xdg`.
- Giao diện có tệp `colors` riêng thì lấy màu nền cửa sổ trong đó. Không có (Breeze `default`) thì
  theo bảng màu hệ thống.
- Ngưỡng tối/sáng giống portal KDE: độ xám dưới 192 là tối. Đọc không ra thì rơi về cách cũ.

**Đo 14/09/2026:**

- Bài kiểm `plasma_panel_theme`, 15 trường hợp. Khai trước là bản rỗng phải đỏ đúng 12 bài (P1–P9,
  S1, E1, E2): ra đúng 12. Mã thật 15/15 xanh, cả bộ 12/12.
- Máy thật Fedora 44 KDE Wayland, chụp khay trước và sau khi cài: V đen thành V trắng.

**Bẫy đo đã gặp:** `strings liblotus.so | grep desktoptheme` ra 0 dù mã đã vào, vì trình biên dịch
nhét chuỗi ngắn thẳng vào lệnh máy. Tìm theo tên hàm bằng `nm -C` mới đúng (đối chứng: tệp kiểm
thấy 2).

**Chưa kiểm:** Kubuntu, Plasma Style của bên thứ ba, đổi giao diện khi Lotus đang chạy (icon đổi ở
lần khay cập nhật kế tiếp, kết quả lưu tạm 5 giây). Chưa gửi upstream. Issue #374 đang mở, tác giả
không dùng KDE và đã mời người dùng KDE gửi bản sửa.

## Cấu hình nên đặt kèm

**Khuyên dùng `Uinput (Super Smooth)` làm chế độ gõ chính** (chủ máy chốt 13/09/2026). Trong
`lotus.conf` phải ghi đúng tên hiển thị `Mode=Uinput (Super Smooth)`; ghi `Mode=SuperSmooth` hay tên
sai khác thì Lotus lặng lẽ quay về Preedit. Trong `lotus-app-rules.conf` dùng số `3`.

**Dự định:** gộp ba chế độ uinput (`Uinput (Smooth)`, `Uinput (Super Smooth)`, `Uinput (Slow)`)
thành một chế độ uinput duy nhất, lấy Super Smooth làm gốc. Chưa làm, chưa có kế hoạch kỹ thuật.

Luật theo app đang dùng trên máy này:

- **`firefox=3`** và **`microsoft-edge=3`** (Super Smooth). Chỉ đặt 3 được khi có vá ở nhóm E. Dùng
  bản Lotus gốc thì đặt `firefox=1` (Smooth), vì Super Smooth gốc không có lá chắn chống lặp chữ ở
  thanh địa chỉ. Lark chạy trong Firefox nên cũng theo luật này.
- **`Alacritty=3`**. Lá chắn chỉ liên quan ô có tự điền, mà cửa sổ dòng lệnh không có tự điền,
  nên Super Smooth không thiệt gì.

## Những gì bản này KHÔNG sửa

Nói rõ để khỏi mất công thử lại:

- **Chế độ Surrounding Text vẫn lỗi.** Cơ chế surrounding text vốn không đáng tin: Firefox đẩy
  văn bản xung quanh trễ 58 đến 184 ms. Đã thử hướng "tin vào bộ đệm" và bị hồi quy ở ô soạn
  thảo giàu, đã rút lại. Chế độ 4 hỏng trên Firefox cũng thuộc nhóm này.
- **Máy chủ uinput chết giữa lúc thay chữ làm bàn phím chết theo.** Đã tái hiện được, bộ đo nằm
  ở `probes/vong_go/may_chu_chet_giua_chung.py` trong repo workbench. Chưa vá, đang trong hàng
  đợi gửi issue.

## Quy ước khi gom lại lần sau

Tác giả đẩy mã rất nhanh, khoảng 163 commit mỗi 30 ngày. Khi cần cập nhật:

**Dựng nhánh MỚI từ `upstream/dev` rồi nhặt lại từng vá.** Đừng gộp chồng lên nhánh cũ. Gộp
chồng làm bản mình tụt lại sau upstream mà không ai để ý, đúng như nhánh `tong-hop` cũ: nó hơn
`dev` 27 commit nhưng lại THIẾU 1 commit của tác giả.

**Trang đầu của fork là `.github/README.md`**, không phải `README.md`. GitHub ưu tiên hiện tệp trong
`.github`, nhờ vậy `README.md` của tác giả giữ nguyên và không gây xung đột mỗi lần gom lại. Trang
đầu chỉ tóm tắt mỗi vá một dòng. **Thêm hoặc bỏ vá thì sửa cả tệp này lẫn trang đầu, trong cùng
commit.**

Sau khi gom xong, phép kiểm bắt buộc là **so mã băm cây mã** với nhánh trước đó. Giống nhau thì
việc gom đúng, khác một byte cũng là hỏng:

```
git rev-parse ban-dung^{tree}
git rev-parse <nhánh cũ>^{tree}
```

Lần gom này: 27 commit rút còn 19, bỏ 4 cặp làm-rồi-rút-lại, cây mã trùng khít
`ca93438c9247d503736bf4ce002d2d799fd252a9`, bộ kiểm 11/11.

**Không đổi tên bản fork.** Chữ `lotus` nằm 1592 chỗ ở 95 tệp, và 4 tệp tác giả sửa nhiều nhất
chính là 4 tệp việc đổi tên phải cày nát, nên đổi tên là tự chuốc xung đột mỗi lần cập nhật.
Gói Nix dùng `--replace-fail` nên đổi tên là gãy bản dựng chứ không phải cảnh báo. Tính lại khi
số vá bị từ chối vượt 8 đến 10 cái.
