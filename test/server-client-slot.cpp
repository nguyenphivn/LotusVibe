// SPDX-License-Identifier: GPL-3.0-or-later
//
// Unit test for ClientSlot: the keyboard socket keeps its first client until that client goes
// away, so a second connection cannot take the channel over from the running fcitx5.

#include "lotus-client-slot.h"

#include <cerrno>
#include <fcntl.h>
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

namespace {

    int  failures = 0;

    void expect(const std::string& name, bool got, bool want) {
        if (got != want) {
            std::cerr << "FAIL: " << name << '\n';
            ++failures;
        }
    }

    bool isClosed(int fd) {
        return fcntl(fd, F_GETFD) < 0 && errno == EBADF;
    }

    int pairEnd() {
        int sv[2] = {-1, -1};
        if (socketpair(AF_UNIX, SOCK_SEQPACKET, 0, sv) != 0) {
            return -1;
        }
        close(sv[1]);
        return sv[0];
    }

} // namespace

int main() {
    const int first  = pairEnd();
    const int second = pairEnd();
    const int third  = pairEnd();
    if (first < 0 || second < 0 || third < 0) {
        std::cerr << "socketpair failed\n";
        return 1;
    }

    ClientSlot slot;
    expect("first client accepted", slot.accept(first), true);
    expect("second client refused while the first is connected", slot.accept(second), false);
    expect("refused fd is closed", isClosed(second), true);
    expect("first client still owns the slot", slot.fd() == first, true);

    slot.drop();
    expect("dropped fd is closed", isClosed(first), true);
    expect("new client accepted after the first left", slot.accept(third), true);

    return failures == 0 ? 0 : 1;
}
