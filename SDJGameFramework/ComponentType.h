#pragma once

// 컴포넌트들의 종류 집합. NONE~END까지 반복문으로 CompoList를 배열에 추가할 수 있음
enum CompoType { NONE, SHAPE, CUSTOM_SHAPE, LIGHT, SCRIPT, END };