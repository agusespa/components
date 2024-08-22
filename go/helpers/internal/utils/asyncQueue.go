package utils

import (
	"sync"
)

type AsyncQueue struct {
	limit       int
	queue       []task
	activeCount int
	mu          sync.Mutex
}

type task struct {
	fn     func() (interface{}, error)
	result chan interface{}
	err    chan error
}

func NewAsyncQueue(limit int) *AsyncQueue {
	return &AsyncQueue{
		limit:       limit,
		queue:       make([]task, 0),
		activeCount: 0,
	}
}

func (aq *AsyncQueue) Enqueue(fn func() (interface{}, error)) (<-chan interface{}, <-chan error) {
	result := make(chan interface{}, 1)
	err := make(chan error, 1)

	aq.mu.Lock()
	aq.queue = append(aq.queue, task{fn: fn, result: result, err: err})
	aq.mu.Unlock()

	go aq.tryRunNext()

	return result, err
}

func (aq *AsyncQueue) tryRunNext() {
	aq.mu.Lock()
	defer aq.mu.Unlock()

	if aq.activeCount < aq.limit && len(aq.queue) > 0 {
		aq.activeCount++
		nextTask := aq.queue[0]
		aq.queue = aq.queue[1:]

		go aq.runTask(nextTask)
	}
}

func (aq *AsyncQueue) runTask(t task) {
	result, err := t.fn()
	if err != nil {
		t.err <- err
		close(t.err)
	} else {
		t.result <- result
		close(t.result)
	}

	aq.mu.Lock()
	aq.activeCount--
	aq.mu.Unlock()

	go aq.tryRunNext()
}
