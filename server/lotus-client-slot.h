/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef _LOTUS_CLIENT_SLOT_H_
#define _LOTUS_CLIENT_SLOT_H_

#include <unistd.h>

/**
 * @brief The single client connection a server socket serves at a time.
 *
 * A new connection is refused while one is open: letting it replace the current client would
 * hand the channel to whichever process connected last, not to the fcitx5 already using it.
 */
class ClientSlot {
  public:
    ClientSlot()                             = default;
    ClientSlot(const ClientSlot&)            = delete;
    ClientSlot& operator=(const ClientSlot&) = delete;
    ~ClientSlot() {
        drop();
    }

    // Takes ownership of fd; closes it when the slot is busy.
    bool accept(int fd) {
        if (connected()) {
            close(fd);
            return false;
        }
        fd_ = fd;
        return true;
    }

    void drop() {
        if (fd_ >= 0) {
            close(fd_);
            fd_ = -1;
        }
    }

    int fd() const {
        return fd_;
    }

    bool connected() const {
        return fd_ >= 0;
    }

  private:
    int fd_ = -1;
};

#endif // _LOTUS_CLIENT_SLOT_H_
