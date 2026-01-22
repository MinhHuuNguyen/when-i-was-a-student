def calc_dis_loss(self, input_fake, input_real):
    # calculate the loss to train D
    outs0 = self.forward(input_fake)
    outs1 = self.forward(input_real)
    loss = 0

    for it, (out0, out1) in enumerate(zip(outs0, outs1)):
        if self.gan_type == 'lsgan':
            loss += torch.mean((out0 - 0)**2) + torch.mean((out1 - 1)**2)
        elif self.gan_type == 'nsgan':
            all0 = Variable(torch.zeros_like(out0.data).cuda(), requires_grad=False)
            all1 = Variable(torch.ones_like(out1.data).cuda(), requires_grad=False)
            loss += torch.mean(F.binary_cross_entropy(F.sigmoid(out0), all0) +
                               F.binary_cross_entropy(F.sigmoid(out1), all1))
        else:
            assert 0, "Unsupported GAN type: {}".format(self.gan_type)
    return loss

def calc_gen_loss(self, input_fake):
    # calculate the loss to train G
    outs0 = self.forward(input_fake)
    loss = 0
    for it, (out0) in enumerate(outs0):
        if self.gan_type == 'lsgan':
            loss += torch.mean((out0 - 1)**2) # LSGAN
        elif self.gan_type == 'nsgan':
            all1 = Variable(torch.ones_like(out0.data).cuda(), requires_grad=False)
            loss += torch.mean(F.binary_cross_entropy(F.sigmoid(out0), all1))
        else:
            assert 0, "Unsupported GAN type: {}".format(self.gan_type)
    return loss

def recon_criterion(self, input, target):
    return torch.mean(torch.abs(input - target))

def compute_vgg_loss(self, vgg, img, target):
    img_vgg = vgg_preprocess(img)
    target_vgg = vgg_preprocess(target)
    img_fea = vgg(img_vgg)
    target_fea = vgg(target_vgg)
    return torch.mean((self.instancenorm(img_fea) - self.instancenorm(target_fea)) ** 2)