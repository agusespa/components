package client

import "testing"

func TestNewExponentialBackoffDefault(t *testing.T) {
	eb := NewExponentialBackoffDefault()

	if eb.InitialInterval != 500 {
		t.Errorf("Expected InitialInterval to be 500, got %d", eb.InitialInterval)
	}

	if eb.Multiplier != 1.5 {
		t.Errorf("Expected Multiplier to be 1.5, got %f", eb.Multiplier)
	}

	if eb.RandomizationFactor != 0.5 {
		t.Errorf("Expected RandomizationFactor to be 0.5, got %f", eb.RandomizationFactor)
	}
}

func TestNewExponentialBackoff(t *testing.T) {
	tests := []struct {
		name                string
		initialInterval     int32
		multiplier          float32
		randomizationFactor float32
	}{
		{"Custom Values", 1000, 2.0, 0.3},
		{"Zero Values", 0, 0, 0},
		{"Mixed Values", 750, 1.75, 0},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			eb := NewExponentialBackoff(tt.initialInterval, tt.multiplier, tt.randomizationFactor)

			if eb.InitialInterval != tt.initialInterval {
				t.Errorf("Expected InitialInterval to be %d, got %d", tt.initialInterval, eb.InitialInterval)
			}

			if eb.Multiplier != tt.multiplier {
				t.Errorf("Expected Multiplier to be %f, got %f", tt.multiplier, eb.Multiplier)
			}

			if eb.RandomizationFactor != tt.randomizationFactor {
				t.Errorf("Expected RandomizationFactor to be %f, got %f", tt.randomizationFactor, eb.RandomizationFactor)
			}
		})
	}
}

// func TestMain(m *testing.M) {
// 	bo := backoff.NewExponentialBackOff()

// 	err := backoff.Retry(func() error {
// 		resp, err := http.Get("https://jsonplaceholder.typicode.com/todos/1")
// 		if err != nil {
// 			return err
// 		}
// 		defer resp.Body.Close()

// 		var result map[string]interface{}
// 		if err := json.NewDecoder(resp.Body).Decode(&result); err != nil {
// 			return err
// 		}

// 		fmt.Printf("%+v\n", result)
// 		return nil
// 	}, bo)

//		if err != nil {
//			fmt.Println("Request failed:", err)
//		}
// }
