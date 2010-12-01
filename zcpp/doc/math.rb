
class Array
  # transpose
  def t
    cols = []
    self[0].each_index do |i|
      cols[i] = map {|v| v[i]}
    end
    cols
  end

  def *(m)
    res = []
    each_index do |row|
      res[row] = []
      m[0].each_index do |col|
        res[row][col] = 0
        self[0].each_index do |i|
          res[row][col] += self[row][i] * m[i][col]
        end
      end
    end
    res
  end

  def det
    (self[0][0] * self[1][1]) - (self[1][0] * self[0][1])
  end

  def /(scalar)
    map {|row| row.map {|v| v / scalar}}
  end

  # vector substraction
  def -(v)
    res = [[]]
    self[0].each_index {|i| res[0][i] = self[0][i] - v[0][i]}
    res
  end

  def to_s
    if self.size > 1 || self[0].size > 1
      if self.size == 1
        row_format = '[' + (" %3.1f" * self[0].size) + " ]\n"
      else
        row_format = '|' + (" %3.1f" * self[0].size) + " |\n"
      end
      self.inject("") {|s,row| s + sprintf(row_format, *row)}
    else
      self[0][0].to_s
    end
  end

  def map_index(&block)
    res = []
    self.each_index do |i|
      res[i] = yield(i,self[i])
    end
    res
  end
end


set = [[1,1],
       [0,4],
       [2,1]]
# mean vector mu
mu = [[]]
set[0].each_index do |i|
  mu[0][i] = set.inject(0) {|s,v| s + v[i] } / set.size.to_f
end

# set - mu
s_mu = set.map do |v|
  r = []
  v.each_index do |i|
    r[i] = v[i] - mu[0][i]
  end
  r
end

# variance-covariance matrix
sigma = (s_mu.t * s_mu) / (set.size - 1.0).to_f

# inverse of sigma
if sigma.det == 0
  puts "Matrix not invertible"
  return
end

# invers of sigma (only for a 2x2 matrix)
sigma_inv = [[  sigma[1][1], -sigma[0][1] ],
             [ -sigma[1][0],  sigma[0][0] ]] / sigma.det

puts "Sigma\n\n"
puts sigma.to_s
puts "\Sigma's inverse\n\n"
puts sigma_inv.to_s

puts "\n\nAll results express value^2 (before square root)"

[
[[[2  , 2  ]],
[ [1  , 3  ]]],
[[[1.5, 1.5]],
[ [1.5, 2.5]]]
].each do |t1,t2|
  puts "\n-----------------------------"
  puts "t1   = #{t1}"
  puts "t2   = #{t2}"
  puts "t1-u = #{t1 - mu}"
  puts "t2-u = #{t2 - mu}\n\n"

  d1 = (t1 - mu).flatten.inject(0) {|s,v| s + v**2} 
  d2 = (t2 - mu).flatten.inject(0) {|s,v| s + v**2}

  puts "d1   = #{d1}"
  puts "d2   = #{d2}\n\n"

  d1nE = (t1 - mu).flatten.map_index{|i,v| v**2 / sigma[i][i] }.inject(0) {|s,v| s + v } 
  d2nE = (t2 - mu).flatten.map_index{|i,v| v**2 / sigma[i][i] }.inject(0) {|s,v| s + v } 

  puts "d1nE = #{d1nE}"
  puts "d2nE = #{d2nE}\n\n"

  d1M = (t1 - mu) * sigma_inv * (t1 - mu).t
  d2M = (t2 - mu) * sigma_inv * (t2 - mu).t

  puts "d1M  = #{d1M}"
  puts "d2M  = #{d2M}\n\n"
end

# ---------------- PCA
d = (8**2 - 4 * (1 * 3))**(0.5)

lambdas = [1,-1].map{|sign| ((8 + (d*sign))/2)}

lambdas.each do |x|
  puts((2-x)*(6-x) -9)
end

ts = lambdas.map{|lambda| [(2-lambda) / (9 + (2 - lambda)**2)**(0.5),lambda]}

xs = ts.map {|t,lambda| (3 * t) / (2 - lambda)}
ys = ts.map {|t,lambda| t}

e = [[xs[0], ys[0]],[xs[1], ys[1]]]
q = e.t * [[1],[0]]
puts "Matrix of eigenvectors:\n#{e}"
puts "T':\n#{s_mu * e.t}"
puts "Q:\n#{q}"
puts "T'':\n#{s_mu * q}"

puts "\nSigma':\n#{(s_mu * e.t).t * (s_mu * e.t)}"
snr0 = (s_mu * e.t).inject(0){|s,r| s + (r[0]**2)}
snr1 = (s_mu * e.t).inject(0){|s,r| s + (r[1]**2)}
puts "SNR0: #{snr0}"
puts "SNR1: #{snr1}"
puts "SNR: #{snr0/snr1}"