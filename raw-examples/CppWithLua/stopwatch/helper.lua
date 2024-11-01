function pT()
    return stopwatch.getPassedTime()
end

function runFor1s()
    local nowTime = now()
    local passedTime = pT()
    print(string.format("%d(%d): runFor1s", nowTime, passedTime))
end

function runFor2s()
    local nowTime = now()
    local passedTime = pT()
    print(string.format("%d(%d): runFor2s", nowTime, passedTime))
end

function runFor5s()
    local nowTime = now()
    local passedTime = pT()
    print(string.format("%d(%d): runFor5s", nowTime, passedTime))
end

function initClocks()
    print('initClocks')
    addClock(1000, 'runFor1s')
    addClock(2000, 'runFor2s')
    stopwatch.addClock(5000, 'runFor5s')
end

module = {}
module.initClocks = initClocks
return module
