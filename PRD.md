# PRD.md — DummyDataGenerator

## 1. 목적

반도체 시료 생산주문관리 시스템(S-Semi) PoC의 핵심 로직 검증을 위해,
실제 운영 데이터 없이 테스트 가능한 JSON 더미 데이터를 생성하는 C++ 콘솔 도구를 개발한다.

생성된 JSON은 S-Semi 메인 시스템의 초기 데이터 로딩 또는 시나리오 단위 테스트에 직접 사용된다.

---

## 2. 사용자 및 사용 맥락

| 사용자 | 사용 목적 |
|--------|-----------|
| S-Semi 개발자 | 메인 시스템의 주문 상태 전이 로직 및 재고 판단 로직 검증 |
| S-Semi 개발자 | 콘솔 UI 동작 확인용 초기 데이터 주입 |

실행 환경: Windows, 개발자 로컬 PC (Visual Studio 2022 빌드 후 콘솔에서 실행)

---

## 3. 생성 대상 엔티티

### 3.1 Sample (시료)

생산 담당자가 시스템에 등록하는 반도체 시료.

| 필드 | 타입 | 설명 |
|------|------|------|
| `sampleId` | string | 시료 고유 식별자 (예: `SMPL-001`) |
| `name` | string | 시료명 (예: `NPN Transistor A`) |
| `stock` | int | 현재 재고 수량 (≥ 0) |

### 3.2 Order (주문서)

주문 담당자가 작성하며, 특정 시료에 대한 수량 요청을 담는다.

| 필드 | 타입 | 설명 |
|------|------|------|
| `orderId` | string | 주문 고유 식별자 (예: `ORD-001`) |
| `customerId` | string | 요청 고객 식별자 (예: `CUST-001`) |
| `sampleId` | string | 요청 시료 식별자 (Sample.sampleId 참조) |
| `quantity` | int | 요청 수량 (≥ 1) |
| `status` | string | 주문 상태 (`RESERVED` \| `REJECTED` \| `PRODUCING` \| `CONFIRMED` \| `RELEASED`) |

### 3.3 ProductionLine (생산 라인)

주문된 시료를 하나씩 생산하는 설비. 재고 부족 시 활성화된다.

| 필드 | 타입 | 설명 |
|------|------|------|
| `lineId` | string | 라인 고유 식별자 (예: `LINE-001`) |
| `assignedOrderId` | string \| null | 현재 생산 중인 주문 ID. 비활성 시 `null` |
| `status` | string | 라인 상태 (`IDLE` \| `RUNNING`) |

---

## 4. 생성 시나리오

각 시나리오는 독립적인 JSON 파일로 출력된다. 파일명은 시나리오 이름을 따른다.

### 시나리오 A — 재고 충분 (happy path)
- 시료 재고가 주문 수량 이상
- 주문 상태: `RESERVED` → `CONFIRMED` → `RELEASED`
- 생산 라인: 미사용 (`IDLE`)
- 출력 파일: `scenario_confirmed.json`

### 시나리오 B — 재고 부족 (생산 중)
- 시료 재고가 주문 수량 미만
- 주문 상태: `RESERVED` → `PRODUCING`
- 생산 라인: 해당 주문에 할당 (`RUNNING`)
- 출력 파일: `scenario_producing.json`

### 시나리오 C — 주문 거절
- 생산 담당자가 주문을 거절
- 주문 상태: `RESERVED` → `REJECTED`
- 생산 라인: 미사용
- 출력 파일: `scenario_rejected.json`

### 시나리오 D — 복합 (혼합 상태)
- 여러 시료, 여러 주문, 각기 다른 상태가 공존
- 메인 시스템의 대시보드/모니터링 뷰 검증 목적
- 출력 파일: `scenario_mixed.json`

---

## 5. 실행 인터페이스

```
DummyDataGenerator.exe [출력_디렉토리]
```

- `출력_디렉토리` 생략 시 실행 파일과 동일한 디렉토리에 출력
- 실행 후 콘솔에 생성된 파일 목록과 각 파일의 레코드 수를 출력
- 정상 종료 시 exit code 0, 파일 쓰기 실패 시 exit code 1

---

## 6. JSON 출력 형식

```json
{
  "scenario": "confirmed",
  "samples": [
    { "sampleId": "SMPL-001", "name": "NPN Transistor A", "stock": 50 }
  ],
  "orders": [
    {
      "orderId": "ORD-001",
      "customerId": "CUST-001",
      "sampleId": "SMPL-001",
      "quantity": 10,
      "status": "CONFIRMED"
    }
  ],
  "productionLines": [
    { "lineId": "LINE-001", "assignedOrderId": null, "status": "IDLE" }
  ]
}
```

- 인코딩: UTF-8
- 들여쓰기: 공백 2칸
- 외부 JSON 라이브러리 사용 금지 — 표준 라이브러리로 직접 직렬화

---

## 7. 범위 외 (Out of Scope)

- 실제 주문 처리 로직 구현 (메인 시스템의 역할)
- 데이터베이스 연동
- 난수 기반 대량 데이터 생성 (시나리오 단위의 고정 데이터만 생성)
- GUI

---

## 8. 완료 기준 (PoC)

- [ ] 4개 시나리오 JSON 파일이 정상 생성됨
- [ ] 생성된 JSON이 메인 시스템에서 파싱 오류 없이 로드됨
- [ ] 각 시나리오의 상태 값이 도메인 정의(섹션 3)와 일치함
