# JokeEngine
DX11을 사용한 게임엔진 제작, Unreal의 ContentBrowser와 Actor, Component를 레퍼런싱하여 구조를 제작

---
### 빌드
이 프로젝트를 빌드하기 위해선 CMAKE 3.24 이상의 버젼이 필요하다.

또한 VisualStudio 2026이 설치되어 있어야 한다.

1. JokeEngine 폴더 안에 빈 폴더를 추가하고 이름은 build로 만든다.

```
  mkdir build
```

2. build 폴더 안에 들어가 cmd 창을 연다(혹은 cmd창에서 cd 명령어를 이용해 JokeEngine\build로 이동한다.)

```
   cd build
```

3. 열린 cmd에서 다음 명령을 실행한다

```
  cmake ..
```

이 과정까지 하면 build 폴더 내에 slnx 솔루션이 생길 것이다. 이 솔루션으로 들어가 빌드를 진행하면 된다.

---
### OpenSource 및 Lisence
Package 폴더 내의 OpenSourceList.md 참고

---
### 엔진 구조 및 사용 설명서
다음 링크의 문서 참고: <https://app.notion.com/p/JokeEngine-3bc2c7365dca80edbcf5cb7b57473e28?source=copy_link>

---
### 현재 계획
- Debug용 imgui 연동
- DWrite, Direct2D를 이용한 텍스트 출력, 그에 따른 JTextObject 추가
- FMOD를 이용한 사운드 모듈 추가(다만 FMOD는 라이센스 비용이 있기에 다른 모듈 사용할 수도 있음)