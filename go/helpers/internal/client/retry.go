package client

type Backoff interface {
	SetInitialInterval(interval int)
	SetMultiplier(multiplier float32)
	SetRandomizationFactor(factor float32)
	Retry(req func())
}

type ExponentialBackoff struct {
	InitialInterval     int32
	Multiplier          float32
	RandomizationFactor float32
}

func NewBackoff() *ExponentialBackoff {
	return &ExponentialBackoff{
		InitialInterval:     500,
		Multiplier:          1.5,
		RandomizationFactor: 0.5,
	}
}

func (b *ExponentialBackoff) SetInitialInterval(interval int32) {
	b.InitialInterval = interval
}

func (b *ExponentialBackoff) SetMultiplier(multiplier float32) {
	b.Multiplier = multiplier
}

func (b *ExponentialBackoff) SetRandomizationFactor(factor float32) {
	b.RandomizationFactor = factor
}

// func Test() {
// 	bo := backoff.NewExponentialBackOff()
// 	bo.InitialInterval = 500 * time.Millisecond
// 	bo.Multiplier = 1.5
// 	bo.RandomizationFactor = 0.5

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

// 	if err != nil {
// 		fmt.Println("Request failed:", err)
// 	}
// }
