
var prev_low_pass = false
function low_pass(newData, alpha)
{
  if(prev_low_pass === false)
  {
    prev_low_pass = newData
  }
  else
  {
    prev_low_pass = prev_low_pass*(1-alpha) + alpha*newData
  }
  return prev_low_pass
}
