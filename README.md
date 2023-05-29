# Valorant
> FPS게임인 발로란트(Valorant)의 시스템을 따라 구현해 보고 Custom Skill을 만들어 본다.

## Tech Stack
<p>
<img src="https://img.shields.io/badge/unrealengine-0E1128?style=for-the-badge&logo=unrealengine&logoColor=white">
<img src="https://img.shields.io/badge/c++-00599C?style=for-the-badge&logo=cplusplus&logoColor=white">
</ p>

## 구현 목록
### Weapon Pick Up & Fire
![총기 줍고 슈팅](https://github.com/minhvvan/Valorant/assets/59609086/e85bc9df-3ccf-40b6-9778-8675221cd668)
- 현재 가지고있지 않은 종류(주무기, 보조무기)라면 바로 Pick Up을 할 수 있게 Pick Up Component를 구현했다.
- 또한 Pick Up이 성공적으로 이루어지면 Input Context와 함수들을 등록하여 총을 발사할 수 있게 구현했다.

### Weapon Drop
![무기 버리기](https://github.com/minhvvan/Valorant/assets/59609086/2511ee9d-7fbe-4405-bd76-efb15e74177a)
- 무기 버리기 Key(F)를 입력하면 현재 위치에 현재 들고 있는 무기를 버리게 구현했다.
- 현재 Player의 위체에서 아래방향으로 LineTrace를 실행하여 Hit된 위치에 Offset을 주어 발앞에 떨어지게 구현했다.

### Weapon Swap
![무기 스왑](https://github.com/minhvvan/Valorant/assets/59609086/4ea4e699-d578-4a38-b95d-7cc4fb5218b8)
- 주무기와 보조무기 모두 소지하고 있을 때 정해진 Key(1: 주무기, 2: 보조무기)를 이용하여 현재 사용할 무기를 변경할 수 있게 구현했다.
- Map 자료구조와 Tag를 이용하여 무기를 분류하였다.
- Input Context를 설정하는 부분이 힘들었다.

### Weapon Change
![무기 변경](https://github.com/minhvvan/Valorant/assets/59609086/4bfa9038-3ee4-48a4-8ba7-1cd72c77e065)
- 상호작용이 가능한 위치에 Player가 접근하면 같은 종류의 무기를 정해진 Key(E)를 이용하여 변경할 수 있게 구현했다.
- Interaction Component를 구현하여 재사용성을 높였다.

### Spike
![스파이크 설치](https://github.com/minhvvan/Valorant/assets/59609086/c16f5d84-4c1c-4f52-9b2a-69b531b0db4d)
- 스파이크는 Level에 단 한개만 존재하기 때문에 소지를 위한 Interaction은 불필요하다.
- 따라서, Pick Up Component를 재사용하여 Player가 소유할 수 있게 구현했다.
- 설치 가능 구역을 Trigger Volume으로 설정한 뒤 Decal을 이용해 표시하였다.
- 설치 가능 구역에 Player가 Overlap하였을 때, Player가 소지한 스파이크를 설치 가능하게 변경하였고 그 상태에서 스파이크를 소지한 Player가 
설치 Key(4)를 정해진 시간동안 누르고 있디면 설치가 완료되게 구현했다.
- 설치가 완료된 스파이크는 Player가 위치했던 곳에 Drop되며 방어팀에서 해제할 수 있는 상태로 변경하였다.

### Hit Check
![피격판정](https://github.com/minhvvan/Valorant/assets/59609086/887507cd-ac59-40a8-9542-a1ff1bc06da0)
- 왼쪽 마우스 클릭을 입력하면 총기를 발사할 수 있게 구현했다.
- Player가 바라보는 방향으로 총기는 발사되게 구현했다. (현재 Player의 위치에서 Forward Vector를 받아와 LineTrace 적용, 사거리 Update 필요)
- LineTrace에 Hit된 Actor가 만약 타격가능한 Actor였다면 Damage를 부여하였다.
- Damage를 받아 체력이 0이 된 Player는 사망하게 구현했다.(Death Animation 적용 필요)
