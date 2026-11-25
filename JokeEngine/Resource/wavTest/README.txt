JokeEngine WAV 로더 테스트 세트
모든 소리: 440Hz 사인파 (stereo는 오른쪽이 660Hz)
OK = XAudio2로 재생돼야 함 / 거부 = 로더가 '지원 안 함'으로 걸러야 함 / 실패 = 깨진 파일

ok_pcm16_mono_fmt16.wav           88244 B  OK | PCM 16bit mono, fmt 16바이트 (cbSize 없음)
ok_pcm16_stereo_fmt18.wav        176446 B  OK | PCM 16bit stereo, fmt 18바이트 (cbSize=0)
ok_pcm8_mono.wav                  44144 B  OK | PCM 8bit (부호 없음, 128이 가운데)
ok_pcm24_stereo_ext.wav          264668 B  OK | PCM 24bit stereo, EXTENSIBLE (SubFormat PCM)
ok_pcm32_mono.wav                176468 B  OK | PCM 32bit 정수 mono, EXTENSIBLE
ok_float32_stereo_fmt18.wav      352858 B  OK | IEEE float 32bit stereo, fmt 18바이트, fact 청크 포함
ok_float32_stereo_ext.wav        352868 B  OK | IEEE float 32bit stereo, EXTENSIBLE (SubFormat FLOAT)
ok_pcm16_48k_stereo.wav          192046 B  OK | PCM 16bit 48000Hz
tricky_junk_before_fmt.wav        88280 B  OK(까다로움) | fmt 앞에 JUNK 청크 -> 건너뛰기 필요
tricky_list_after_data.wav       176482 B  OK(까다로움) | data 뒤에 LIST(INFO) 청크
tricky_odd_data_pad.wav           44182 B  OK(까다로움) | data 크기가 홀수 -> 패딩 1바이트 뒤에 LIST
tricky_unknown_odd_chunk.wav      88264 B  OK(까다로움) | 모르는 청크(afsp) 크기 홀수 -> 패딩
tricky_short_100ms.wav             8864 B  OK | 0.1초짜리 짧은 효과음
unsupported_alaw.wav               8058 B  거부 | A-law (태그 6), 8000Hz
unsupported_ulaw.wav               8058 B  거부 | mu-law (태그 7), 8000Hz
unsupported_alaw_ext.wav          16080 B  거부 | EXTENSIBLE인데 SubFormat Data1=6 (A-law) <- 오늘 그 파일과 같은 형태
unsupported_float64.wav          352858 B  거부 | IEEE float 64bit (double)
unsupported_mp3_tag.wav            1082 B  거부 | 태그 0x55 (MP3 in WAV) - 내용은 가짜, 태그 검사용
unsupported_ima_adpcm_tag.wav      1072 B  거부 | 태그 0x11 (IMA ADPCM) - 내용은 가짜, 태그 검사용
broken_not_riff.wav                  44 B  실패 | RIFF로 시작 안 함
broken_not_wave.wav                  36 B  실패 | RIFF지만 WAVE가 아님
broken_no_data.wav                   36 B  실패 | data 청크 없음
broken_no_fmt.wav                 88220 B  실패 | fmt 청크 없음
broken_truncated_data.wav         44122 B  주의 | 파일이 중간에 잘림 (data 크기 > 실제 남은 바이트). 잘라서 재생하거나 실패
broken_fmt_too_small.wav          88234 B  실패 | fmt 크기 6바이트 (16 미만)
broken_empty_file.wav                 0 B  실패 | 0바이트 파일
