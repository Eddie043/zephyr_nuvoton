
# SPDX-License-Identifier: Apache-2.0

board_runner_args(jlink "--device=npcm400f" "--speed=4000")
board_runner_args(jlink "--bin-file=./build/zephyr/${CONFIG_KERNEL_BIN_NAME}_signed.bin")
board_runner_args(jlink "--reset-after-load")
include(${ZEPHYR_BASE}/boards/common/jlink.board.cmake)
