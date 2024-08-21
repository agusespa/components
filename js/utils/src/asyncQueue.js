export function createQueue(tasks, maxNumOfWorkers = 4) {
    let numOfWorkers = 0;
    let taskIndex = 0;

    return new Promise(done => {
        const handleResult = index => result => {
            tasks[index] = result;
            numOfWorkers--;
            getNextTask();
        };
        const getNextTask = () => {
            console.log('getNextTask numOfWorkers=' + numOfWorkers);
            if (numOfWorkers < maxNumOfWorkers && taskIndex < tasks.length) {
                tasks[taskIndex]().then(handleResult(taskIndex)).catch(handleResult(taskIndex));
                taskIndex++;
                numOfWorkers++;
                getNextTask();
            } else if (numOfWorkers === 0 && taskIndex === tasks.length) {
                done(tasks);
            }
        };
        getNextTask();
    });
}

export const createTask = value => () => {
    if (value === 6) return Promise.reject(new Error('sorry'));
    return new Promise(resolve => setTimeout(() => resolve(value), value));
};
