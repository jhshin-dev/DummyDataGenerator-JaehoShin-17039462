# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Purpose

**DummyDataGenerator**는 반도체 시료 생산주문관리 시스템(S-Semi) PoC의 핵심 로직 검증을 위한 JSON 형태의 더미 데이터를 생성하는 C++ 콘솔 도구이다.

## Build

- IDE: Visual Studio 2022 (solution: `DummyDataGenerator.slnx`, project: `DummyDataGenerator.vcxproj`)
- 언어 표준: C++20 (`stdcpp20`)
- Toolset: v145 (VS 2022)
- 타겟: x64 Debug / Release (주 빌드), x86 병행 지원

**MSBuild 빌드 (CLI):**
```
msbuild DummyDataGenerator.vcxproj /p:Configuration=Debug /p:Platform=x64
```

**실행 파일 출력 경로:** `x64\Debug\DummyDataGenerator.exe` (Debug 기준)

별도의 외부 라이브러리 없이 표준 라이브러리만 사용한다. JSON 직렬화도 외부 라이브러리 없이 직접 구현한다.

## Domain Model

### 역할 (Roles)
| 역할 | 설명 |
|------|------|
| 고객 (Customer) | 시료 요청자. 이메일로 주문 담당자에게 요청 |
| 주문 담당자 (Order Manager) | 주문서 작성 및 생산 담당자에게 전달 |
| 생산 담당자 (Production Manager) | 시료 등록, 주문 승인/거절, 재고 관리 |

### 주문 상태 흐름 (Order State Flow)
```
RESERVED → [승인] → 재고 확인
                       ├─ 재고 충분 → CONFIRMED → RELEASED
                       └─ 재고 부족 → PRODUCING → (생산 완료) → CONFIRMED → RELEASED
           [거절] → REJECTED  ※ 모니터링 제외 상태
```

| 상태 | 의미 |
|------|------|
| `RESERVED` | 주문 접수 |
| `REJECTED` | 주문 거절 (비정상 흐름) |
| `PRODUCING` | 승인 완료, 재고 부족으로 생산 중 |
| `CONFIRMED` | 승인 완료, 출고 대기 |
| `RELEASED` | 출고 완료 |

### 핵심 엔티티
- **Sample (시료)**: 생산 담당자가 등록하는 반도체 시료. 품번, 재고 수량을 보유
- **Order (주문서)**: 주문 담당자가 작성. 특정 시료에 대한 수량 요청 포함
- **ProductionLine (생산 라인)**: 시료를 하나씩 생산. 주문된 시료에 대해서만 동작

## JSON Output 구조

생성되는 JSON은 시스템의 초기 상태 또는 특정 시나리오를 표현하는 스냅샷이다.

```json
{
  "samples": [...],
  "orders": [...],
  "productionLines": [...]
}
```

각 시나리오(정상 흐름 전체, 재고 충분, 재고 부족, 거절 등)를 별도 JSON 파일로 출력하는 것을 권장한다.

## Architecture Guidelines (PoC)

- 외부 JSON 라이브러리 금지 — 표준 라이브러리(`<fstream>`, `<sstream>`, `<string>`)로 직접 직렬화
- 핵심 검증 목적이므로 과도한 추상화나 디자인 패턴 적용 지양
- 각 엔티티(Sample, Order, ProductionLine)는 간단한 struct 또는 class로 표현
- 시나리오 생성 함수는 독립적으로 호출 가능하도록 분리
- 출력 경로는 실행 인수로 받거나 실행 파일과 동일한 디렉토리에 저장
