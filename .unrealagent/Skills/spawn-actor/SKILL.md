---
name: spawn-actor
description: 레벨에 액터를 생성합니다
disable-model-invocation: true
---

사용자가 요청한 액터를 현재 레벨에 생성합니다.

## 절차

1. 인자에서 클래스명과 위치를 파악합니다. 위치가 없으면 원점(0,0,0)을 사용합니다.
2. `execute_python`으로 액터를 스폰합니다.
3. 사용자가 추가 속성(스케일, 회전, 머티리얼 등)을 요청하면 `set_editor_property()`로 설정합니다.
4. 생성 결과(이름, 위치, 클래스)를 요약하여 보고합니다.

## 참고

- StaticMeshActor, PointLight, CameraActor 등 기본 클래스는 `unreal.` 접두사로 접근합니다.
- 커스텀 블루프린트는 `unreal.EditorAssetLibrary.load_blueprint_class()`로 로드합니다.
- 메시를 지정하려면 스폰 후 `static_mesh_component.set_editor_property("static_mesh", asset)`으로 설정합니다.
