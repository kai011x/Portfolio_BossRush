# Move Control Logic to BossRushPlayerController

이 계획서는 `ACharacterBase`에 구현되어 있던 입력 액션 변수 및 입력 바인딩 로직을 `ABossRushPlayerController`로 이관하여 컨트롤러가 입력 처리를 담당하도록 구조를 변경하는 계획입니다.

## User Review Required

> [!IMPORTANT]
> 1. **블루프린트 설정 갱신 필요**: 입력 액션 에셋(`UInputAction*`)들이 `CharacterBase`가 아닌 플레이어 컨트롤러(`BossRushPlayerController`)의 블루프린트에서 바인딩되도록 변수 위치가 이동됩니다. 리팩토링 후 에디터 내 플레이어 컨트롤러 블루프린트에서 입력 액션 에셋을 다시 지정해 주셔야 합니다.
> 2. **네트워크 복제**: 입력 바인딩은 로컬 컨트롤러에서 실행되며, 서버/클라이언트 환경에 맞게 캐릭터의 기존 네트워크 함수(어빌리티 시스템 등)를 호출하도록 처리합니다.

## Proposed Changes

---

### BossRush Component

#### [MODIFY] [BossRushPlayerController.h](file:///D:/Portfolio/BossRush/Source/BossRush/Public/Characters/BossRushPlayerController.h)
- `InputActionValue.h` 헤더를 포함합니다.
- `UInputAction` 멤버 변수 선언들을 `CharacterBase.h`로부터 가져옵니다 (`JumpAction`, `MoveAction`, `LookAction`, `NormalAction`, `SubAction`, `UltraSkill`, `NormalSkill`, `DashAction`, `SprintAction`, `TargetingAction`).
- `SetupInputComponent()` 메소드를 오버라이드 선언합니다.
- 각 입력 액션에 대응하는 콜백 함수(바인딩용 핸들러)들을 선언합니다.

#### [MODIFY] [BossRushPlayerController.cpp](file:///D:/Portfolio/BossRush/Source/BossRush/Private/Characters/BossRushPlayerController.cpp)
- `SetupInputComponent()`를 구현하고, 플레이어의 EnhancedInputComponent를 통해 각각의 InputAction과 콜백을 바인딩합니다.
- 바인딩된 각 콜백에서 Possess 중인 캐릭터(`ACharacterBase`)를 가져와 대응하는 동작 함수를 호출합니다.

#### [MODIFY] [CharacterBase.h](file:///D:/Portfolio/BossRush/Source/BossRush/Public/Characters/CharacterBase.h)
- 기존 입력 관련 UPROPERTY 변수들을 제거합니다. (단, `DefaultMappingContext` 변수는 컨트롤러로 일원화하고 캐릭터에서는 제거합니다.)
- `SetupPlayerInputComponent()` 선언을 유지하되 내부 바인딩은 제거하며, `NotifyControllerChanged()` 오버라이드를 제거합니다.

#### [MODIFY] [CharacterBase.cpp](file:///D:/Portfolio/BossRush/Source/BossRush/Private/Characters/CharacterBase.cpp)
- `SetupPlayerInputComponent()` 내 기존 EnhancedInput 바인딩 코드를 삭제합니다.
- `NotifyControllerChanged()` 함수 구현을 삭제합니다.

---

## Verification Plan

### Manual Verification
1. 코드를 빌드하고 언리얼 에디터를 켭니다.
2. 플레이어 컨트롤러 블루프린트에서 각 입력 액션 변수(`JumpAction`, `MoveAction` 등)가 정상적으로 표시되는지 확인하고, 해당 에셋들을 바인딩합니다.
3. 게임을 실행하여 이동, 카메라 회전, 점프, 대시, 일반 공격, 질주, 타겟팅 등 모든 입력 동작이 정상적으로 캐릭터에게 적용되는지 확인합니다.
