멀티채널 테스트: 채널이 순서대로 하나씩 0.6초씩 소리 납니다 (각 채널 다른 음 높이).
스테레오 장치에서 재생하면 XAudio2가 다운믹스해서, 어느 쪽에서 들리는지로 배치를 확인할 수 있어요.
LFE(저음)는 60Hz라서 작은 스피커/이어폰에선 거의 안 들릴 수 있어요.

multi_3ch_FL_FR_FC.wav       595418 B  OK | 3채널 (2.1이 아니라 앞 L/R + 센터) | 채널 순서: FL FR FC
multi_4ch_quad.wav          1058468 B  OK | 4채널 쿼드 | 채널 순서: FL FR BL BR
multi_6ch_5_1_back.wav      2381468 B  OK | 5.1 (뒤 스피커, mask 0x3F) | 채널 순서: FL FR FC LFE BL BR
multi_6ch_5_1_side.wav      2381468 B  OK | 5.1 (옆 스피커, mask 0x60F) | 채널 순서: FL FR FC LFE SL SR
multi_8ch_7_1.wav           4233668 B  OK | 7.1 (mask 0x63F) | 채널 순서: FL FR FC LFE BL BR SL SR
multi_6ch_5_1_float32.wav   4762868 B  OK | 5.1 float 32bit | 채널 순서: FL FR FC LFE BL BR
multi_6ch_plain_noext.wav   2381446 B  주의 | 6채널인데 EXTENSIBLE 아님 (채널 배치 정보 없음, 규격 위반에 가까움) | 채널 순서: FL FR FC LFE BL BR
