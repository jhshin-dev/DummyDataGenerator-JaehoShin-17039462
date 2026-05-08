# DummyDataGenerator

반도체 시료 생산주문관리 시스템 **S-Semi** PoC의 핵심 로직 검증을 위한 JSON 더미 데이터 생성 도구.

---

## 개요

실제 운영 데이터 없이 S-Semi 메인 시스템의 주문 상태 전이 로직 및 재고 판단 로직을 테스트할 수 있도록, 시나리오 단위의 JSON 스냅샷을 생성한다.

## 빌드 환경

| 항목 | 값 |
|------|----|
| IDE | Visual Studio 2022 |
| 언어 표준 | C++20 |
| 타겟 플랫폼 | x64 (주), x86 |
| 외부 라이브러리 | 없음 (표준 라이브러리만 사용) |

**MSBuild CLI:**
```
msbuild DummyDataGenerator.vcxproj /p:Configuration=Debug /p:Platform=x64
```

실행 파일 출력 경로: `x64\Debug\DummyDataGenerator.exe`

## 실행

```
DummyDataGenerator.exe [출력_디렉토리]
```

- `출력_디렉토리` 생략 시 실행 파일과 동일한 디렉토리에 출력
- 정상 종료: exit code `0` / 파일 쓰기 실패: exit code `1`

**실행 예시:**
```
DummyDataGenerator.exe C:\output

DummyDataGenerator — S-Semi PoC
Output: "C:\output"

[OK]   scenario_confirmed.json  (samples=2, orders=2, lines=2)
[OK]   scenario_producing.json  (samples=1, orders=1, lines=1)
[OK]   scenario_rejected.json  (samples=1, orders=1, lines=1)
[OK]   scenario_mixed.json  (samples=3, orders=5, lines=2)
```

## 생성 시나리오

| 파일 | 시나리오 | 주문 상태 | 생산 라인 |
|------|---------|----------|----------|
| `scenario_confirmed.json` | 재고 충분 | `CONFIRMED`, `RELEASED` | `IDLE` |
| `scenario_producing.json` | 재고 부족 | `PRODUCING` | `RUNNING` (주문 연결) |
| `scenario_rejected.json` | 주문 거절 | `REJECTED` | `IDLE` |
| `scenario_mixed.json` | 혼합 (대시보드 검증용) | 5가지 상태 혼재 | `RUNNING`, `IDLE` |

## 도메인 모델

### 주문 상태 흐름

```
RESERVED → [승인] → 재고 충분 → CONFIRMED → RELEASED
                  → 재고 부족 → PRODUCING → (생산 완료) → CONFIRMED → RELEASED
         → [거절] → REJECTED
```

### JSON 구조

```json
{
  "scenario": "mixed",
  "samples": [
    { "sampleId": "SMPL-001", "name": "NPN Transistor A", "stock": 30 }
  ],
  "orders": [
    {
      "orderId": "ORD-001", "customerId": "CUST-001",
      "sampleId": "SMPL-001", "quantity": 10, "status": "RELEASED"
    }
  ],
  "productionLines": [
    { "lineId": "LINE-001", "assignedOrderId": "ORD-002", "status": "RUNNING" },
    { "lineId": "LINE-002", "assignedOrderId": null, "status": "IDLE" }
  ]
}
```

## 소스 구조

| 파일 | 역할 |
|------|------|
| `models.h` | `Sample`, `Order`, `ProductionLine` struct 및 상태 enum 정의 |
| `json_writer.h` | 표준 라이브러리 기반 JSON 직렬화 (header-only) |
| `scenarios.h/cpp` | 4개 시나리오 데이터 생성 함수 |
| `main.cpp` | CLI 진입점 및 파일 출력 |
